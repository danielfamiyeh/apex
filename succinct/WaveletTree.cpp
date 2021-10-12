//
// Created by daniel on 11/10/2021.
//

#include <map>

#include <utility>

#include <vector>

#include <deque>

#include <iostream>

#include <set>

#include "WaveletTree.h"

void
printTree(
    WaveletNode * v
) {
  BitVector < bool > val = v -> getValue();
  char c = v -> getLeafValue();
  if (c) std::cout << c;
  else {
    for (auto && i: val.getVector()) {
      std::cout << i;
    }
  }
  std::cout << std::endl;
}

WaveletTree::WaveletTree(
    std::string alphabet,
    const std::string & str
) {

  root = partition(std::move(alphabet), str, true);
  getCodes();
  //print();
}

WaveletTree::~WaveletTree() {
  delete(root);
}

WaveletNode *
WaveletTree::partition(
    std::string alphabet,
    const std::string & str,
    bool start = false
) {
  int alphaMidpoint = (int)(alphabet.size() / 2);
  std::map < char, bool > charMap;
  BitVector < bool > bitVector;
  std::vector < char > leftStrVector;
  std::vector < char > rightStrVector;
  std::set < char > leftAlpha;
  std::set < char > rightAlpha;
  WaveletNode* v = nullptr;

  // Create 0/1 character map
  for (int i = 0; i < alphabet.size(); i++) {
    charMap[alphabet[i]] = i >= alphaMidpoint;
    codes[alphabet[i]].push_back(i >= alphaMidpoint);
  }

  // Covert string to bitVector
  for (char c: str) {
    bool val = (bool) charMap[c];
    bitVector.pushBack(val);

    // Create new child vectors and alphabet
    if (!val) {
      leftStrVector.push_back(c);
      leftAlpha.insert(c);
    } else {
      rightStrVector.push_back(c);
      rightAlpha.insert(c);
    }
  }

  v = new WaveletNode(bitVector);

  if (alphabet.size() == 1) {
    v -> setLeafValue(alphabet[0]);
    leaves.push_back(v);
    return v;
  }

  // Recurse on children and assign
  v -> setLeftChild(partition(
      std::string(leftAlpha.begin(), leftAlpha.end()),
      std::string(leftStrVector.begin(), leftStrVector.end())
          ));
  v -> getLeftChild() -> setEdgeValue(0);

  v -> setRightChild(partition(
      std::string(rightAlpha.begin(), rightAlpha.end()),
      std::string(rightStrVector.begin(), rightStrVector.end())
          ));
  v -> getRightChild() -> setEdgeValue(1);

  return v;
}

void
    WaveletTree::getCodes() {
  for (auto current : leaves) {
      std::deque<bool> code;
      WaveletNode* v;
      WaveletNode* n = current;

      while(n->getParent()) {
        v = n;
        code.push_front(v->getEdgeValue());
        n = v->getParent();
      }
      codes[current->getLeafValue()] = code;
      std::cout << current->getLeafValue() << ": ";
      for(auto c : code){
        std::cout << c;
      }
      std::cout << std::endl;
    }
}

void
WaveletTree::preorder(
    WaveletNode * v,
    void( * fun)(WaveletNode * )
) {
  if (!v) return;
  fun(v);
  preorder(v -> getLeftChild(), fun);
  preorder(v -> getRightChild(), fun);
}

void
WaveletTree::print() {
  preorder(root, printTree);
}

char
WaveletTree::access(int i) {
  int * _i = new int(i);
  WaveletNode * n = root;
  while (!n -> getLeafValue()) {
    BitVector < bool > bVec = n -> getValue();
    bool bBool = bVec.access( * _i);
    n = !bBool ? n -> getLeftChild() : n -> getRightChild();
    * _i = bVec.rank(bBool, * _i);
  }

  delete(_i);
  return n -> getLeafValue();
}

int
rank(char c, int i) {

}