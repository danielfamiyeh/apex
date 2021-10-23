//
// Created by daniel on 11/10/2021.
//

#ifndef DNASSEMBLER_WAVELETTREE_H
#define DNASSEMBLER_WAVELETTREE_H

#include <deque>

#include <vector>

#include <unordered_map>

#include "../util/BitVector.h"

#include "../util/ISuccinctStructure.h"

#include "Vertex.h"

typedef Vertex<BitVector<bool>, std::string, bool> WaveletNode;

class WaveletTree : public ISuccinctStructure<std::string> {
private:
  std::unordered_map<std::string, std::deque<bool> *> codes;
  std::vector<WaveletNode *> leaves;
  WaveletNode *root;
  WaveletNode *partition(std::vector<std::string> alphabet,
                         const std::vector<std::string>& str);
  void inferCodes();

public:
  WaveletTree(std::vector<std::string> alphabet, const std::vector<std::string>& str);
  ~WaveletTree();

  void preorder(WaveletNode *v, void (*fun)(WaveletNode *));

  void postorder(WaveletNode *v, void (*fun)(WaveletNode *));

  void print();

  std::string access(int i) override;

  int rank(std::string c, int i) override;

  int select(std::string c, int i) override;
};

#endif // DNASSEMBLER_WAVELETTREE_H