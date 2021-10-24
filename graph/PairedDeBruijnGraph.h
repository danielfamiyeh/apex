//
// Created by daniel on 23/10/2021.
//

#ifndef DNASSEMBLER_PAIREDDEBRUIJNGRAPH_H
#define DNASSEMBLER_PAIREDDEBRUIJNGRAPH_H

#include "DeBruijnGraph.h"

class PairedDeBruijnGraph {
  typedef struct flag {
    explicit flag(bool State=false, int IndexTo = -1, int IndexFrom = -1) {
      state = new bool(State);
      indexTo = new int(IndexTo);
      indexFrom = new int(IndexFrom);
    }
    bool *state;
    int *indexTo;
    int *indexFrom;
  } flag_t;

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
  std::vector<unsigned long> linkers;
  std::vector<flag_t> forwardFlags;
  std::vector<flag_t> reverseFlags;


public:
  PairedDeBruijnGraph(const std::string &path1, const std::string &path2,
                      int K);
};

#endif // DNASSEMBLER_PAIREDDEBRUIJNGRAPH_H
