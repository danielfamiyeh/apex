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

private:
  int k;
  std::vector<std::string> nodes;
  BitVector<bool> *first;
  WaveletTree *w;

public:
  BWT(int _k, const std::string& path);
  ~BWT();
};

#endif // DNASSEMBLER_BWT_H
