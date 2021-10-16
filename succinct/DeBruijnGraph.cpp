//
// Created by daniel on 13/10/2021.
//

#include <fstream>

#include <iostream>

#include <algorithm>
#include <vector>

#include "DeBruijnGraph.h"

// For co-lex. sorting node matrix with edge label vector
typedef struct nodeWithEdge {
  nodeWithEdge(std::string NodeLabel, char EdgeLabel) {
    nodeLabel = NodeLabel;
    edgeLabel = EdgeLabel;
  }

  std::string nodeLabel;
  char edgeLabel;
} nodeWithEdge_t;

DeBruijnGraph::DeBruijnGraph(int k, const std::string &path) {
  w = nullptr;
  last = new BitVector<bool>;
  first["$"] = 0;
  std::vector<std::string> nodes;

  std::ifstream reads(path);
  if (reads.is_open()) {
    std::vector<char> _w;
    std::vector<nodeWithEdge> nodesWithEdges;

    for (std::string read; getline(reads, read);) {
      read.insert(0, std::string(k, '$'));
      read.insert(read.size(), "$");

      for (int i = 0; i < read.size() - (k); i++) {
        std::string kmer = read.substr(i, k);
        char edgeLabel = read[k + i];
        std::reverse(kmer.begin(), kmer.end());
        bool exists =
            std::find_if(nodesWithEdges.begin(), nodesWithEdges.end(),
                         [&edgeLabel, &kmer](const nodeWithEdge &nwe) -> bool {
                           return nwe.nodeLabel == kmer &&
                                  nwe.edgeLabel == edgeLabel;
                         }) != nodesWithEdges.end();

        if (!exists)
          nodesWithEdges.emplace_back(kmer, edgeLabel);
      }
    }

    // Co-lex sort nodes and W tree
    std::sort(
        nodesWithEdges.begin(), nodesWithEdges.end(),
        [](auto const &a, auto const &b) { return a.nodeLabel < b.nodeLabel; });
    for (auto &nodesWithEdge : nodesWithEdges) {
      std::reverse(nodesWithEdge.nodeLabel.begin(),
                   nodesWithEdge.nodeLabel.end());
      nodes.push_back(nodesWithEdge.nodeLabel);
      _w.push_back(nodesWithEdge.edgeLabel);
    }

    std::vector<std::string> alphabet{"A", "T", "C", "G", "$"};
    //    std::cout << std::string(_w.begin(), _w.end()) << std::endl;
    w = new WaveletTree(alphabet, std::string(_w.begin(), _w.end()));

    // F vector
    for (int i = 0; i < nodes.size() - 1; i++) {
      last->pushBack(nodes[i] != nodes[i + 1]);
    }
    last->pushBack(true);

    // Flag setting
    for (int i = 0; i < nodes.size(); i++) {
      flags.emplace_back(false);
      if (i > 0) {
        for (int j = 0; j < i; j++) {
          if (w->access(j) == w->access(i) &&
              nodes[j].substr(1, k - 1) == nodes[i].substr(1, k - 1)) {
            *flags[i].state = true;
            *flags[i].indexTo = j;
            *flags[j].indexFrom = i;
          }
        }

        // F vector
        std::string after = nodes[i].substr(k - 1, 1);
        if (after != nodes[i - 1].substr(k - 1, 1)) {
          first[after] = i;
        }
      }
    }

    //            for (int i = 0; i < nodes.size(); i++) {
    //              std::cout << last->access(i) << " " << nodes[i] << " " <<
    //              w->access(i)
    //                        << (*flags[i].state ? "-" : "") << std::endl;
    //            }
  } else {
    std::cout << "Could not open file " << path << ".\n";
  }
}

int DeBruijnGraph::forward(int u, bool isOutgoing) {
  std::string c = w->access(u);
  int *_u = new int(*flags[u].state && !isOutgoing ? *flags[u].indexTo : u);
  int *rankC = new int(w->rank(c, *_u));

  /*
   * Since flags and edge labels are stored in separate structures
   * once W.rank(c, *_u) is calculated, traverse the interval [0, _u)
   * if W[i] == c and a flag is set at i then W[i] ∈ A⁻ ==> W[i] != c
   * so decrement rankC since it was counted in W.rank(c, *_u) originally.
   */
  for (int i = 0; i < *_u; i++) {
    if (*flags[i].state && w->access(i) == c) {
      *rankC -= 1;
    }
  }

  int startPosition = first[c];
  int rankToBase = last->rank(true, startPosition);
  int nodeIndex = last->select(true, (*rankC + rankToBase));

  delete _u;
  delete rankC;

  return nodeIndex;
}

int DeBruijnGraph::backward(int v) {
  std::string c = new char('$');

  for (auto &it : first) {
    if (v >= it.second)
      c = it.first;
  }

  int rankToBase = last->rank(true, first[c]);
  int rankToCurrentEdge = last->rank(true, (v));
  int index = rankToCurrentEdge - rankToBase;

  /*
   * When calculating the edge index, w.select() won't take into
   * account flagged edges. We define an offset and increment for every
   * flagged c over the interval [0, w.select(c, index+ 1)).
   * This ensures that we skip past flagged edges on the final
   * select call.
   */

  int *indexOffset = new int(0);
  for (int i = 0; i < w->select(c, index + 1); i++) {
    if (*flags[i].state && w->access(i) == c) {
      *indexOffset += 1;
    }
  }

  int edge = w->select(c, index + *indexOffset);
  return edge;
}

int DeBruijnGraph::outdegree(int v) {
  return last->select(true, v) - last->select(true, v - 1);
}

int DeBruijnGraph::outgoing(int v, const std::string &c) {
  int *x = new int(w->select(c, w->rank(c, v)));
  int *nodeIndex = new int(last->select(true, v));
  int *nodeIndexBefore = new int(last->select(true, v - 1));
  bool *edgeExists = new bool(false);

  if (*x > *nodeIndexBefore && *x <= *nodeIndex) *edgeExists = true;
  else {
    int *_v = new int(-1);
    for(int i=(v-1); i>=0; i--) {
      if(*flags[i].state != *flags[v].state && w->access(i) == w->access(v)) {
        *_v = i;
        break;
      }
    }
    *x = w->select(c, w->rank(c, *_v));
    *nodeIndex = last->select(true, *_v);
    *nodeIndexBefore = last->select(true, *_v - 1);


    if (*x > *nodeIndexBefore && *x <= *nodeIndex) *edgeExists = true;
  }
  int edge = *x;
  bool _edgeExists = *edgeExists;

  delete x;
  delete nodeIndex;
  delete nodeIndexBefore;
  delete edgeExists;

  return _edgeExists ? forward(edge, true) : -1;
}