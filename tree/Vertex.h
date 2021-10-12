//
// Created by daniel on 10/10/2021.
//

#ifndef DNASSEMBLER_VERTEX_H
#define DNASSEMBLER_VERTEX_H

template <typename T, typename T2> class Vertex {

private:
  Vertex *left;
  Vertex *right;
  Vertex *parent;
  T *value;
  T2 *leafValue;
  T2 *edgeValue;

public:
  explicit Vertex(T val);
  ~Vertex();
  Vertex<T, T2> *getLeftChild();
  Vertex<T, T2> *getRightChild();
  Vertex<T, T2> *getParent();
  T getValue();
  T2 getLeafValue();
  T2 getEdgeValue();

  void setLeftChild(Vertex<T, T2> *lv);
  void setRightChild(Vertex<T, T2> *rv);
  void setParent(Vertex<T, T2> *pv);
  void setValue(T val);
  void setEdgeValue(T2 edgeVal);
  void setLeafValue(T2 val);
};

#endif // DNASSEMBLER_VERTEX_H
