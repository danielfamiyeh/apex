//
// Created by daniel on 10/10/2021.
//

#include "Tree.h"
#include "Vertex.h"

template <typename T, typename T2>
Tree<T, T2>::Tree(){
    root = nullptr;
}

template <typename T, typename T2>
Vertex<T, T2>* Tree<T, T2>::getRoot() {
    return root;
}

template <typename T, typename T2>
Vertex<T, T2>* Tree<T, T2>::getVertex(T val) {
    Vertex<T, T2>* next = root;
    Vertex<T, T2>* current;

    while(next != nullptr){
        current = next;
        next = val < current->getValue() ? current->getLeftChild() : current->getRightChild();
        if(current->getValue() == val) return current;
    }
    return nullptr;
}


template <typename T, typename T2>
Vertex<T, T2>* Tree<T, T2>::getParent(T val) {
    Vertex<T, T2>* target = getVertex(val);
    return target == nullptr ? target : target->getParent();
}

template <typename T, typename T2>
Vertex<T, T2>** Tree<T, T2>::getChildren(T val) {
    Vertex<T, T2>* target = getVertex(val);

    if(target)
    {
        Vertex<T, T2> children[2] = {target->getLeftChild(), target->getRightChild()};
        return children;
    }
    return nullptr;
}

template <typename T, typename T2>
bool Tree<T, T2>::isInternal(T val) {
    Vertex<T, T2>* target = getVertex(val);
    return target != nullptr && (target->getLeftChild() || target->getRightChild());
}

template <typename T, typename T2>
bool Tree<T, T2>::isExternal(T val) {
    return !isInternal(val);
}

template <typename T, typename T2>
bool Tree<T, T2>::isEmpty() {
    return !root;
}

template <typename T, typename T2>
bool Tree<T, T2>::isRoot(T val) {
    Vertex<T, T2>* target = getVertex(val);
    return target == root;
}

template <typename T, typename T2>
bool Tree<T, T2>::find(T val) {
    return getVertex(val);
}

