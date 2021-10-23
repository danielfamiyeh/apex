//
// Created by daniel on 10/10/2021.
//

#ifndef DNASSEMBLER_VERTEX_H
#define DNASSEMBLER_VERTEX_H

template <typename T, typename T2, typename T3> class Vertex {

private:
  Vertex *left;
  Vertex *right;
  Vertex *parent;
  T *value;
  T2 *leafValue;
  T3 *edgeValue;

public:
  explicit Vertex(T val);
  ~Vertex();
  Vertex<T, T2, T3> *getLeftChild();
  Vertex<T, T2, T3> *getRightChild();
  Vertex<T, T2, T3> *getParent();
  T getValue();
  T2 getLeafValue();
  T3 getEdgeValue();

  void setLeftChild(Vertex<T, T2, T3> *lv);
  void setRightChild(Vertex<T, T2, T3> *rv);
  void setParent(Vertex<T, T2, T3> *pv);
  void setValue(T val);
  void setEdgeValue(T3 edgeVal);
  void setLeafValue(T2 val);
};

#endif // DNASSEMBLER_VERTEX_H
