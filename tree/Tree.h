//
// Created by daniel on 10/10/2021.
//

#ifndef DNASSEMBLER_GRAPH_H
#define DNASSEMBLER_GRAPH_H


#include "Vertex.h"

template <typename T, typename T2>
class Tree {
private:
    Vertex<T, T2>* root;

public:
    Tree();
    Vertex<T, T2>* getRoot();
    Vertex<T, T2>* getVertex(T val);
    Vertex<T, T2>* getParent(T val);
    Vertex<T, T2>** getChildren(T val);
    bool isInternal(T val);
    bool isExternal(T val);
    bool isEmpty();
    bool isRoot(T val);
    bool find(T val);
};


#endif //DNASSEMBLER_GRAPH_H
