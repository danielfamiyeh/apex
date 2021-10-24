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
                unsigned int LinkIndex = -1) {
    nodeLabel = std::move(NodeLabel);
    edgeLabel = std::move(EdgeLabel);
    linkIndex = LinkIndex;
  }

  std::vector<std::string> nodeLabel;
  std::string edgeLabel;
  unsigned int linkIndex;
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

  std::cout << forwardRead << " | " << reverseRead << std::endl;
}