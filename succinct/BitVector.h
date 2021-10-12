//
// Created by daniel on 11/10/2021.
//

#ifndef DNASSEMBLER_BITVECTOR_H
#define DNASSEMBLER_BITVECTOR_H

#include <iterator>
#include <vector>

#include "ISuccinctStructure.h"

template <typename T>

class BitVector : public ISuccinctStructure<T> {

private:
  std::vector<T> vector;

public:
  BitVector();

  T access(int i);

  int rank(T c, int i);

  int select(T c, int i);

  void pushBack(T c);

  void print();

  std::vector<T> getVector();
};

#endif // DNASSEMBLER_BITVECTOR_H
