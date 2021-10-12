//
// Created by daniel on 11/10/2021.
//

#ifndef DNASSEMBLER_WAVELETTREE_H
#define DNASSEMBLER_WAVELETTREE_H

#include <deque>

#include <vector>

#include <unordered_map>

#include "BitVector.h"

#include "ISuccinctStructure.h"

#include "../tree/Vertex.h"

typedef Vertex < BitVector < bool > , char > WaveletNode;

class WaveletTree {
private:
  std::unordered_map < char, std::deque< bool >> codes;
  std::vector<WaveletNode* > leaves;
  Vertex < BitVector < bool > , char > * root;
  Vertex < BitVector < bool > , char > *
  partition(
      std::string alphabet,
      const std::string & str, bool start
  );
  void
  getCodes();
public:
  WaveletTree(
      std::string alphabet,
      const std::string & str
  );
  ~WaveletTree();

  void
  preorder(
      Vertex < BitVector < bool > , char > * v,
      void( * fun)(Vertex < BitVector < bool > , char > * )
  );

  void
  print();

  char
  access(int i);

  int
  rank(char c, int i);

  //    int select(T c, int i);
};

#endif //DNASSEMBLER_WAVELETTREE_H