//
// Created by daniel on 23/10/2021.
//

#ifndef DNASSEMBLER_PAIREDDEBRUIJNGRAPH_H
#define DNASSEMBLER_PAIREDDEBRUIJNGRAPH_H

#include "DeBruijnGraph.h"

class PairedDeBruijnGraph {
private:
  unsigned int k;
  WaveletTree *forwardEdges;
  WaveletTree *reverseEdges;
  BitVector<bool> *forwardLast;
  BitVector<bool> *reverseLast;
  std::map<std::string, int> forwardFirst;
  std::map<std::string, int> reverseFirst;
  int numNodes;
  int forwardNumReads;
  int reverseNumReads;

public:
  PairedDeBruijnGraph(const std::string &path1, const std::string &path2,
                      int K);
};

#endif // DNASSEMBLER_PAIREDDEBRUIJNGRAPH_H
