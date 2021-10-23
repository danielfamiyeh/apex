//
// Created by daniel on 10/10/2021.
//

#include "Vertex.h"

#include "../util/BitVector.h"

// Constructor
template <typename T, typename T2, typename T3>
Vertex<T, T2, T3>::Vertex(T val) {
  left = nullptr;
  right = nullptr;
  parent = nullptr;
  value = new T(val);
  leafValue = new T2();
  edgeValue = new T3();
}

template <typename T, typename T2, typename T3> Vertex<T, T2, T3>::~Vertex() {
  delete (value);
  delete (edgeValue);
  delete (leafValue);
}

// Getters
template <typename T, typename T2, typename T3>
T Vertex<T, T2, T3>::getValue() {
  return *value;
}

template <typename T, typename T2, typename T3>
T3 Vertex<T, T2, T3>::getEdgeValue() {
  return *edgeValue;
}

template <typename T, typename T2, typename T3>
Vertex<T, T2, T3> *Vertex<T, T2, T3>::getLeftChild() {
  return left;
}

template <typename T, typename T2, typename T3>
Vertex<T, T2, T3> *Vertex<T, T2, T3>::getRightChild() {
  return right;
}

template <typename T, typename T2, typename T3>
Vertex<T, T2, T3> *Vertex<T, T2, T3>::getParent() {
  return parent;
}

template <typename T, typename T2, typename T3>
T2 Vertex<T, T2, T3>::getLeafValue() {
  return *leafValue;
}

// Setters
template <typename T, typename T2, typename T3>
void Vertex<T, T2, T3>::setValue(T val) {
  *value = val;
}

template <typename T, typename T2, typename T3>
void Vertex<T, T2, T3>::setEdgeValue(T3 edgeVal) {
  *edgeValue = edgeVal;
}

template <typename T, typename T2, typename T3>
void Vertex<T, T2, T3>::setLeftChild(Vertex<T, T2, T3> *lv) {
  left = lv;
  lv->parent = this;
}

template <typename T, typename T2, typename T3>
void Vertex<T, T2, T3>::setRightChild(Vertex<T, T2, T3> *rv) {
  right = rv;
  rv->parent = this;
}

template <typename T, typename T2, typename T3>
void Vertex<T, T2, T3>::setParent(Vertex<T, T2, T3> *pv) {
  parent = pv;
}

template <typename T, typename T2, typename T3>
void Vertex<T, T2, T3>::setLeafValue(T2 val) {
  *leafValue = val;
}

template class Vertex<BitVector<bool>, std::string, bool>;