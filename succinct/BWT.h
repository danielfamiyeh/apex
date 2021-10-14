//
// Created by daniel on 13/10/2021.
//

#ifndef DNASSEMBLER_BWT_H
#define DNASSEMBLER_BWT_H

#include <vector>

#include <string>

#include "BitVector.h"

#include "WaveletTree.h"

class BWT {

typedef struct flag {
    flag(bool State, int Index=-1){
      state = new bool(State);
      index = new int(Index);
    }

    bool *state;
    int *index;
} flag_t;

private:
  int k;
  std::vector<std::string> nodes;
  std::vector<flag_t> flags;
  BitVector<bool> *last;
  WaveletTree *w;

public:
  BWT(int _k, const std::string& path);
  ~BWT();
};

#endif // DNASSEMBLER_BWT_H
