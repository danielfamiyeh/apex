//
// Created by daniel on 13/10/2021.
//

#ifndef DNASSEMBLER_DBG_H
#define DNASSEMBLER_DBG_H

#include <vector>

#include <string>

#include <map>

#include "BitVector.h"

#include "WaveletTree.h"

class DBG {

  typedef struct flag {
    explicit flag(bool State, int Index = -1) {
      state = new bool(State);
      index = new int(Index);
    }

    bool *state;
    int *index;
  } flag_t;

private:
  int k;
  std::vector<flag_t> flags;
  std::map<std::string, int> first;
  BitVector<bool> *last;
  WaveletTree *w;

public:
  DBG(int k, const std::string &path);
  int forward(int u);
  int backward(int v);
  ~DBG();

  int outdegree(int v);
};

#endif // DNASSEMBLER_DBG_H
