//
// Created by daniel on 11/10/2021.
//

#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <set>

#include "WaveletTree.h"

void
printTree(
  Vertex<BitVector<bool>,
  char>* v
  )
{
  BitVector<bool> val = v->getValue();
  char c = v->getEdgeValue();
  if (c) std::cout << c;
  else {
    for (auto &&i: val.getVector()) {
      std::cout << i;
    }
  }
  std::cout << std::endl;
}

WaveletTree::WaveletTree(
        std::string alphabet,
        const std::string& str
        )
{
  root = partition(std::move(alphabet), str, true);
  print();
}

WaveletTree::~WaveletTree() {
  delete(root);
}

Vertex<BitVector<bool>, char>*
WaveletTree::partition(
  std::string alphabet,
  const std::string& str,
  bool start=false
)
{
    int alphaMidpoint = (int)(alphabet.size() /2);
    std::map<char, bool> charMap;
    BitVector<bool> bitVector;
    std::vector<char> leftStrVector;
    std::vector<char> rightStrVector;
    std::set<char> leftAlpha;
    std::set<char> rightAlpha;
    Vertex<BitVector<bool>, char>* v = nullptr;

    // Create 0/1 character map
    for (int i=0; i<alphabet.size(); i++) {
        charMap[alphabet[i]] = i >= alphaMidpoint;
    }

    // Covert string to bitVector
    for (char c : str) {
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

    v = new Vertex<BitVector<bool>, char>(bitVector);

    if(alphabet.size() == 1) {
        v->setEdgeValue(alphabet[0]);
        return v;
    }

    // Recurse on children and assign
   v->setLeftChild(partition(
            std::string(leftAlpha.begin(), leftAlpha.end()),
            std::string(leftStrVector.begin(), leftStrVector.end())
            ));

   v->setRightChild(partition(
            std::string(rightAlpha.begin(), rightAlpha.end()),
            std::string(rightStrVector.begin(), rightStrVector.end())
    ));

    return v;
}

void
WaveletTree::preorder(
  Vertex<BitVector<bool>, char>* v,
  void (*fun)(Vertex<BitVector<bool>, char>*)
        )
{
  if (!v) return;
  fun(v);
  preorder(v->getLeftChild(), fun);
  preorder(v->getRightChild(), fun);
}

void
WaveletTree::print()
{
  preorder(root, printTree);
}