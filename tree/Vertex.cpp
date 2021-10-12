//
// Created by daniel on 10/10/2021.
//

#include "Vertex.h"

#include "../succinct/BitVector.h"

// Constructor
template < typename T, typename T2 >
Vertex < T, T2 > ::Vertex(T val) {
  left = nullptr;
  right = nullptr;
  parent = nullptr;
  value = new T(val);
  leafValue = new T2();
  edgeValue = new T2();

}

template < typename T, typename T2 >
Vertex < T, T2 > ::~Vertex() {
  delete(value);
  delete(edgeValue);
  delete(leafValue);
}

// Getters
template < typename T, typename T2 >
T Vertex < T, T2 > ::getValue() {
  return *value;
}

template < typename T, typename T2 >
T2 Vertex < T, T2 > ::getEdgeValue() {
  return *edgeValue;
}

template < typename T, typename T2 >
Vertex < T, T2 > * Vertex < T, T2 > ::getLeftChild() {
  return left;
}

template < typename T, typename T2 >
Vertex < T, T2 > * Vertex < T, T2 > ::getRightChild() {
  return right;
}

template < typename T, typename T2 >
Vertex < T, T2 > * Vertex < T, T2 > ::getParent() {
  return parent;
}

template < typename T, typename T2 >
T2 Vertex < T, T2 > ::getLeafValue() {
  return *leafValue;
}

// Setters
template < typename T, typename T2 >
void Vertex < T, T2 > ::setValue(T val) {
  * value = val;
}

template < typename T, typename T2 >
void Vertex < T, T2 > ::setEdgeValue(T2 edgeVal) {
  * edgeValue = edgeVal;
}

template < typename T, typename T2 >
void Vertex < T, T2 > ::setLeftChild(Vertex < T, T2 > * lv) {
  left = lv;
  lv->parent = this;
}

template < typename T, typename T2 >
void Vertex < T, T2 > ::setRightChild(Vertex < T, T2 > * rv) {
  right = rv;
  rv->parent = this;
}

template < typename T, typename T2 >
void Vertex < T, T2 > ::setParent(Vertex < T, T2 > * pv) {
  parent = pv;
}

template < typename T, typename T2 >
void Vertex < T, T2 > ::setLeafValue(T2 val) {
  * leafValue = val;
}

template class Vertex < BitVector < bool > , char > ;