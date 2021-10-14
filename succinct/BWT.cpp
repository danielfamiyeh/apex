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
  first = new BitVector<bool>;

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

    // Co-lex sort
    std::sort(
        nodesWithEdges.begin(), nodesWithEdges.end(),
        [](auto const &a, auto const &b) { return a.nodeLabel < b.nodeLabel; });
    for (auto & nodesWithEdge : nodesWithEdges) {
      std::reverse(nodesWithEdge.nodeLabel.begin(),
                   nodesWithEdge.nodeLabel.end());
      nodes.push_back(nodesWithEdge.nodeLabel);
      _w.push_back(nodesWithEdge.edgeLabel);
    }

    std::vector<std::string> alphabet{"A", "T", "C", "G", "$"};
    std::cout << std::string(_w.begin(), _w.end()) << std::endl;
    w = new WaveletTree(alphabet, std::string(_w.begin(), _w.end()));
    w->print();

    for(int i=0; i<nodes.size()-1; i++) {
      first->pushBack(nodes[i] != nodes[i+1]);
    }
    first->pushBack(true);

  } else {
    std::cout << "Could not open file " << path << ".\n";
  }
}