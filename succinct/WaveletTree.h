//
// Created by daniel on 11/10/2021.
//

#ifndef DNASSEMBLER_WAVELETTREE_H
#define DNASSEMBLER_WAVELETTREE_H

#include <vector>

#include "ISuccinctStructure.h"
#include "../tree/Vertex.h"

class WaveletTree{
private:
    Vertex<std::vector<bool>, char>* root;
    Vertex<std::vector<bool>, char>* partition(std::string alphabet, const std::string& str, bool start);
public:
    WaveletTree(std::string alphabet, const std::string& str);
    ~WaveletTree();


    void preorder(Vertex<std::vector<bool>, char>* v, void (*fun)(Vertex<std::vector<bool>, char>*));
    void print();
//    T access(int i);
//    int rank(T c, int i);
//    int select(T c, int i);
};


#endif //DNASSEMBLER_WAVELETTREE_H
