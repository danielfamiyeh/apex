//
// Created by daniel on 23/10/2021.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#include "PairedDeBruijnGraph.h"

typedef struct tentativeNode {

  tentativeNode(std::vector<std::string> NodeLabel, std::string EdgeLabel,
                unsigned long LinkIndex = -1) {
    nodeLabel = std::move(NodeLabel);
    edgeLabel = std::move(EdgeLabel);
    linkIndex = *new unsigned long(LinkIndex);
  }

  std::vector<std::string> nodeLabel;
  std::string edgeLabel;
  unsigned long linkIndex;
} tentativeNode_t;

PairedDeBruijnGraph::PairedDeBruijnGraph(const std::string &path1,
                                         const std::string &path2, int K) {

  std::fstream forwardData(path1);
  std::fstream reverseData(path2);
  std::string metaForward, metaReverse, forwardRead, reverseRead;

  std::vector<std::string> forwardAlphabet{"A", "T", "C", "G", "N"};
  std::vector<std::string> reverseAlphabet{"A", "T", "C", "G", "N"};
  k = K;
  numNodes = 0;
  forwardNumReads = 0;
  reverseNumReads = 0;
  forwardEdges = nullptr;
  reverseEdges = nullptr;
  forwardLast = new BitVector<bool>;
  reverseLast = new BitVector<bool>;

  std::vector<tentativeNode_t> forwardNodesWithEdges;
  std::vector<tentativeNode_t> reverseNodesWithEdges;

  //  while (getline(forwardData, forward) && getline(reverseData, reverse)) {
  //  }

  getline(forwardData, metaForward);
  getline(reverseData, metaReverse);
  getline(forwardData, forwardRead);
  getline(reverseData, reverseRead);

  std::vector<std::vector<std::string>> forwardNodes, reverseNodes;
  std::vector<std::string> _forwardEdges, _reverseEdges;

  // Pad reads
  forwardRead.insert(0, std::string(k, '$'));
  reverseRead.insert(0, std::string(k, '$'));
  forwardRead.insert(forwardRead.size(), "$");
  reverseRead.insert(reverseRead.size(), "$");

  std::string forwardPad = "$" + std::to_string(forwardNumReads);
  std::string reversePad = "$" + std::to_string(reverseNumReads);

  // Add pad to first map
  forwardFirst[forwardPad] = forwardNumReads;
  reverseFirst[reversePad] = reverseNumReads;

  // Add pad to alphabet
  forwardAlphabet.push_back(forwardPad);
  reverseAlphabet.push_back(reversePad);

  // K-mer construction
  for (int i = 0; i < forwardRead.size() - k && i < reverseRead.size() - k;
       i++) {
    if (i == forwardRead.size() - k - 1)
      forwardNumReads++;
    if (i == reverseRead.size() - k - 1)
      reverseNumReads++;

    // Get k-mer substring
    std::string forwardSubstr = forwardRead.substr(i, k);
    std::string reverseSubstr = reverseRead.substr(i, k);

    std::vector<std::string> forwardKmer, reverseKmer;

    forwardKmer.reserve(k);
    reverseKmer.reserve(k);

    // Add characters to kmer - adding index to pad
    for (int j = 0; j < k; j++) {
      forwardKmer.push_back(forwardSubstr[j] == '$'
                                ? forwardPad
                                : std::string(1, forwardSubstr[j]));
      reverseKmer.push_back(reverseSubstr[j] == '$'
                                ? reversePad
                                : std::string(1, reverseSubstr[j]));
    }

    // Get edge labels from k + 1 character of node
    std::string forwardEdgeLabel = forwardRead[k + i] == '$'
                                       ? forwardPad
                                       : std::string(1, forwardRead[k + i]);
    std::string reverseEdgeLabel = reverseRead[k + i] == '$'
                                       ? reversePad
                                       : std::string(1, reverseRead[k + i]);

    // Reverse k-mers
    std::reverse(forwardKmer.begin(), forwardKmer.end());
    std::reverse(reverseKmer.begin(), reverseKmer.end());

    bool existsInForwardGraph =
        std::find_if(forwardNodesWithEdges.begin(), forwardNodesWithEdges.end(),
                     [&forwardKmer, &forwardEdgeLabel](
                         const tentativeNode_t &tentNode) -> bool {
                       return tentNode.nodeLabel == forwardKmer &&
                              tentNode.edgeLabel == forwardEdgeLabel;
                     }) != forwardNodesWithEdges.end();

    bool existsInReverseGraph =
        std::find_if(reverseNodesWithEdges.begin(), reverseNodesWithEdges.end(),
                     [&reverseKmer, &reverseEdgeLabel](
                         const tentativeNode_t &tentNode) -> bool {
                       return tentNode.nodeLabel == reverseKmer &&
                              tentNode.edgeLabel == reverseEdgeLabel;
                     }) != reverseNodesWithEdges.end();

    if (!existsInForwardGraph) {
      forwardNodesWithEdges.emplace_back(forwardKmer, forwardEdgeLabel, i);
      forwardNumReads++;
    }

    if (!existsInReverseGraph) {
      reverseNodesWithEdges.emplace_back(reverseKmer, reverseEdgeLabel, i);
      reverseNumReads++;
    }
  }

  /**
   * First sort reverse strand nodes and W tree by co-lex
   * ordering of nodes breaking tie-breaks via edge labels
   */
  std::sort(reverseNodesWithEdges.begin(), reverseNodesWithEdges.end(),
            [this](auto const &a, auto const &b) {
              for (int i = 0; i < k; i++) {
                if (a.nodeLabel[i] == b.nodeLabel[i])
                  continue;
                return a.nodeLabel[i] < b.nodeLabel[i];
              }
              return a.edgeLabel < b.edgeLabel;
            });

  /**
   * For reverse strand store edge labels in tentative W and node labels
   * Modify link indexes for forward strand to match sorted reverse strands
   */
  for (int i = 0; i < reverseNodesWithEdges.size(); i++) {
    auto &nodesWithEdge = reverseNodesWithEdges[i];

    std::reverse(nodesWithEdge.nodeLabel.begin(),
                 nodesWithEdge.nodeLabel.end());
    reverseNodes.push_back(nodesWithEdge.nodeLabel);
    _reverseEdges.push_back(nodesWithEdge.edgeLabel);

    forwardNodesWithEdges[nodesWithEdge.linkIndex].linkIndex = i;
  }

  /**
   * Then co-lex sort forward strand add to corresponding data structures
   */
  std::sort(forwardNodesWithEdges.begin(), forwardNodesWithEdges.end(),
            [this](auto const &a, auto const &b) {
              for (int i = 0; i < k; i++) {
                if (a.nodeLabel[i] == b.nodeLabel[i])
                  continue;
                return a.nodeLabel[i] < b.nodeLabel[i];
              }
              return a.edgeLabel < b.edgeLabel;
            });

  for (auto &nodesWithEdge : forwardNodesWithEdges) {
    std::reverse(nodesWithEdge.nodeLabel.begin(),
                 nodesWithEdge.nodeLabel.end());
    forwardNodes.push_back(nodesWithEdge.nodeLabel);
    _forwardEdges.push_back(nodesWithEdge.edgeLabel);

    linkers.push_back(nodesWithEdge.linkIndex);
  }

  // W Trees
  forwardEdges = new WaveletTree(forwardAlphabet, _forwardEdges);
  reverseEdges = new WaveletTree(reverseAlphabet, _reverseEdges);

  // L Vectors
  bool *forwardFlag = new bool(false);
  bool *reverseFlag = new bool(false);

  for (int i = 0; i < forwardNodes.size(); i++) {
    if (i == forwardNodes.size() - 1) {
      forwardLast->pushBack(true);
      reverseLast->pushBack(true);
      continue;
    }

    for (int j = 0; j < k && !*forwardFlag && !*reverseFlag; j++) {
      if (forwardNodes[i][j] != forwardNodes[i + 1][j])
        *forwardFlag = true;
      if (reverseNodes[i][j] != reverseNodes[i + 1][j])
        *reverseFlag = true;
    }

    forwardLast->pushBack(*forwardFlag);
    reverseLast->pushBack(*reverseFlag);

    *forwardFlag = false;
    *reverseFlag = false;

    forwardFlags.emplace_back(false);
    reverseFlags.emplace_back(false);

    if (i > 0) {
      for (int j = 0; j < i; j++) {
        if(forwardEdges->access(j) == forwardEdges->access(i)) {
          *forwardFlag = true;

          for(int l = 1; l<k && *forwardFlag; l++) {
            if(forwardNodes[j][l] != forwardNodes[i][l]) {
              *forwardFlag = false;
            }
          }
        }

        if(reverseEdges->access(j) == reverseEdges->access(i)) {
          *reverseFlag = true;

          for(int l = 1; l<k && *reverseFlag; l++) {
            if(reverseNodes[j][l] != reverseNodes[i][l]) {
              *reverseFlag = false;
            }
          }
        }

        if(*forwardFlag) {
          *forwardFlags[i].state = true;
          *forwardFlags[i].indexTo = j;
          *forwardFlags[j].indexFrom = i;
          *forwardFlag = false;
        }

        if(*reverseFlag) {
          *reverseFlags[i].state = true;
          *reverseFlags[i].indexTo = j;
          *reverseFlags[j].indexFrom = i;
          *reverseFlag = false;
        }
      }

      if(forwardNodes[i][k-1] != forwardNodes[i-1][k-1])
        forwardFirst[forwardNodes[i][k-1]] = i;

      if(reverseNodes[i][k-1] != reverseNodes[i-1][k-1])
        reverseFirst[reverseNodes[i][k-1]] = i;
    }
  }

  for(auto flag: reverseFlags) {
    std::cout << *flag.state;
  }

  std::cout << "\n";

  delete forwardFlag;
  delete reverseFlag;
  std::cout << forwardRead << " | " << reverseRead << std::endl;
}