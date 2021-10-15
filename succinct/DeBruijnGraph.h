//
// Created by daniel on 13/10/2021.
//

#ifndef DNASSEMBLER_DEBRUIJNGRAPH_H
#define DNASSEMBLER_DEBRUIJNGRAPH_H

#include <vector>

#include <string>

#include <map>

#include "BitVector.h"

#include "WaveletTree.h"

class DeBruijnGraph {

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
  int k;
  std::vector<flag_t> flags;
  std::map<std::string, int> first;
  BitVector<bool> *last;
  WaveletTree *w;

public:
  DeBruijnGraph(int k, const std::string &path);
  int forward(int u, bool isOutgoing=false);
  int backward(int v);
  ~DeBruijnGraph();

  int outdegree(int v);
  int outgoing(int v, const std::string& c);
  int indegree(int v);
};

#endif // DNASSEMBLER_DEBRUIJNGRAPH_H