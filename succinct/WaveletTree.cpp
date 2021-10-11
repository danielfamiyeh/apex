//
// Created by daniel on 11/10/2021.
//

#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <set>

#include "WaveletTree.h"
void _print(Vertex<std::vector<bool>, char>* v) {
    std::vector<bool> val = v->getValue();
    for(auto && i : val) {
        std::cout << i;
    }
    std::cout << std::endl;
}
WaveletTree::WaveletTree(std::string alphabet, const std::string& str) {
    root = partition(std::move(alphabet), str, true);
    print();
}

WaveletTree::~WaveletTree() {
    delete(root);
}

Vertex<std::vector<bool>, char>* WaveletTree::partition(std::string alphabet, const std::string& str, bool start=false) {

    int alphaMidpoint = (int)(alphabet.size() /2);
    std::map<char, bool> charMap;
    std::vector<bool> bitVector;
    std::vector<char> leftStrVector;
    std::vector<char> rightStrVector;
    std::set<char> leftAlpha;
    std::set<char> rightAlpha;
    Vertex<std::vector<bool>, char>* v = nullptr;

    // Create 0/1 character map
    for (int i=0; i<alphabet.size(); i++) {
        charMap[alphabet[i]] = i >= alphaMidpoint;
    }

    // Covert string to bitVector
    for (char c : str) {
        bool val = (bool)charMap[c];
        bitVector.push_back(val);

        // Create new child vectors and alphabet
        if(!val) {
            leftStrVector.push_back(c);
            leftAlpha.insert(c);
        }
        else {
            rightStrVector.push_back(c);
            rightAlpha.insert(c);
        }
    }

    if(alphabet.size() == 1) return nullptr;

    v = new Vertex<std::vector<bool>, char>(bitVector);

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

void WaveletTree::preorder(Vertex<std::vector<bool>, char>* v, void (*fun)(Vertex<std::vector<bool>, char>*)) {
    if (!v) return;
    fun(v);
    preorder(v->getLeftChild(), fun);
    preorder(v->getRightChild(), fun);

}

void WaveletTree::print(){
    preorder(root, _print);
}