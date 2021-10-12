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

void printTree(WaveletNode *v) {
  BitVector<bool> val = v->getValue();
  std::string c = v->getLeafValue();
  if (!c.empty())
    std::cout << c;
  else {
    for (auto &&i : val.getVector()) {
      std::cout << i;
    }
  }
  std::cout << std::endl;
}

WaveletTree::WaveletTree(std::vector<std::string> alphabet, const std::string
                                                            &str) {
  root = partition(alphabet, str, true);
  inferCodes();
}

WaveletTree::~WaveletTree() { delete (root); }

WaveletNode *WaveletTree::partition(std::vector<std::string> alphabet,
                                    const std::string &str,
                                    bool start = false) {
  int alphaMidpoint = (int)(alphabet.size() / 2);
  std::map<std::string , bool> charMap;
  BitVector<bool> bitVector;
  std::vector<char> leftStrVector;
  std::vector<char> rightStrVector;
  std::set<std::string> leftAlpha;
  std::set<std::string> rightAlpha;
  WaveletNode *v = nullptr;

  // Create 0/1 character map
  for (int i = 0; i < alphabet.size(); i++) {
    charMap[alphabet[i]] = i >= alphaMidpoint;
  }

  // Covert string to bitVector
  for (char c : str) {
    std::string charAsStr = std::string(1, c);
    bool val = (bool)charMap[charAsStr];
    bitVector.pushBack(val);

    // Create new child vectors and alphabet
    if (!val) {
      leftStrVector.push_back(c);
      leftAlpha.insert(charAsStr);
    } else {
      rightStrVector.push_back(c);
      rightAlpha.insert(charAsStr);
    }
  }

  v = new WaveletNode(bitVector);

  if (alphabet.size() == 1) {
    v->setLeafValue(alphabet[0]);
    codes[alphabet[0]] = new std::deque<bool>();
    leaves.push_back(v);
    return v;
  }

  // Recurse on children and assign
  v->setLeftChild(
      partition(std::vector<std::string>(leftAlpha.begin(), leftAlpha.end()),
                std::string(leftStrVector.begin(), leftStrVector.end())));
  v->getLeftChild()->setEdgeValue(false); // aka 0

  v->setRightChild(
      partition(std::vector<std::string>(rightAlpha.begin(), rightAlpha.end()),
                std::string(rightStrVector.begin(), rightStrVector.end())));
  v->getRightChild()->setEdgeValue(true); // aka 1

  return v;
}

void WaveletTree::inferCodes() {
  for (auto current : leaves) {
    WaveletNode *v;
    WaveletNode *n = current;

    while (n->getParent()) {
      v = n;
      n = v->getParent();
      codes[current->getLeafValue()]->push_front((v->getEdgeValue()));
    }
  }
}

void WaveletTree::preorder(WaveletNode *v, void (*fun)(WaveletNode *)) {
  if (!v)
    return;
  fun(v);
  preorder(v->getLeftChild(), fun);
  preorder(v->getRightChild(), fun);
}

void WaveletTree::print() { preorder(root, printTree); }

std::string WaveletTree::access(int i) {
  int *_i = new int(i);
  WaveletNode *n = root;

  while (n->getLeafValue().empty()) {
    BitVector<bool> bVec = n->getValue();
    bool bBool = bVec.access(*_i);
    n = !bBool ? n->getLeftChild() : n->getRightChild();
    *_i = bVec.rank(bBool, *_i);
  }

  delete (_i);
  return n->getLeafValue();
}

int WaveletTree::rank(std::string c, int i) {
  int k = 0;
  int *_i = new int(i);
  WaveletNode *n = root;

  while (n->getLeafValue().empty()) {
    BitVector<bool> bVec = n->getValue();
    bool bBool = codes[c]->at(k);
    n = bBool ? n->getRightChild() : n->getLeftChild();
    *_i = bVec.rank(bBool, *_i);
    k++;
  }
  return *_i;
}

int WaveletTree::select(std::string c, int i) {
  int *_i = new int(i);
  WaveletNode *n = nullptr;

  // walk down to leaf
  for (int j = 0; j < leaves.size() && !n; j++) {
    if (leaves[i]->getLeafValue() == c)
      n = leaves[i];
  }
  if (!n)
    return -1;

  unsigned long k = codes[c]->size() - 1;

  while (n->getParent()) {
    n = n->getParent();
    BitVector<bool> bVec = n->getValue();
    bool bBool = codes[c]->at(k);
    *_i = bVec.select(bBool, *_i);
    k--;
  }
  return *_i;
}