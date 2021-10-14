//
// Created by daniel on 13/10/2021.
//

#include <fstream>

#include <iostream>

#include <algorithm>
#include <vector>

#include "BWT.h"

// For co-lex. sorting node matrix with edge label vector
typedef struct nodeWithEdge {
  nodeWithEdge(std::string NodeLabel, char EdgeLabel) {
    nodeLabel = NodeLabel;
    edgeLabel = EdgeLabel;
  }

  std::string nodeLabel;
  char edgeLabel;
} nodeWithEdge_t;

BWT::BWT(int _k, const std::string &path) {
  k = _k;
  w = nullptr;
  last = new BitVector<bool>;

  std::ifstream reads(path);
  if (reads.is_open()) {
    std::vector<char> _w;
    std::vector<nodeWithEdge> nodesWithEdges;

    for (std::string read; getline(reads, read);) {
      read.insert(0, std::string(k, '$'));
      read.insert(read.size(), "$");

      for (int i = 0; i < read.size() - (k); i++) {
        std::string kmer = read.substr(i, k);
        std::reverse(kmer.begin(), kmer.end());
        nodesWithEdges.emplace_back(kmer, read[k + i]);
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
    std::cout << std::string(_w.begin(), _w.end()) << std::endl;
    w = new WaveletTree(alphabet, std::string(_w.begin(), _w.end()));

    // F vector
    for (int i = 0; i < nodes.size() - 1; i++) {
      last->pushBack(nodes[i] != nodes[i + 1]);
    }
    last->pushBack(true);

    for (int i = 0; i < nodes.size(); i++)
      flags.push_back(false);

    for (int i = 1; i < nodes.size(); i++) {
      for (int j = 0; j < i; j++) {
        if (w->access(j) == w->access(i) &&
            nodes[j].substr(1, k - 1) == nodes[i].substr(1, k - 1)) {
          flags[i] = true;
        }
      }
    }

    std::cout << "F "
              << "Nodes "
              << "W" << std::endl;

    for (int i = 0; i < nodes.size(); i++) {
      std::cout << last->access(i) << " " << nodes[i] << " " << w->access(i)
                << (flags[i] ? "-" : "") << std::endl;
    }
  } else {
    std::cout << "Could not open file " << path << ".\n";
  }
}