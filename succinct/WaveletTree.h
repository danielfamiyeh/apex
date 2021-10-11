//
// Created by daniel on 11/10/2021.
//

#ifndef DNASSEMBLER_WAVELETTREE_H
#define DNASSEMBLER_WAVELETTREE_H

#include <vector>

#include "BitVector.h"
#include "ISuccinctStructure.h"
#include "../tree/Vertex.h"

class WaveletTree{
private:
  Vertex<BitVector<bool>, char>* root;
  Vertex<BitVector<bool>, char>*
  partition(
    std::string alphabet,
    const std::string& str, bool start
    );
public:
  WaveletTree(
          std::string alphabet,
          const std::string& str
          );
  ~WaveletTree();


  void
  preorder(
    Vertex<BitVector<bool>, char>* v,
    void (*fun)(Vertex<BitVector<bool>, char>*)
    );

  void print();
//    T access(int i);
//    int rank(T c, int i);
//    int select(T c, int i);
};


#endif //DNASSEMBLER_WAVELETTREE_H
