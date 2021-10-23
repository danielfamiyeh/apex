//
// Created by daniel on 11/10/2021.
//

#include <iostream>
#include <vector>

#include "BitVector.h"

template <typename T> BitVector<T>::BitVector() = default;

template <typename T> BitVector<T>::BitVector(T data[], int n) {
  std::copy(data, data + n, std::back_inserter(vector));

}

template <typename T> T BitVector<T>::access(int i) { return vector[i]; }

template <typename T> int BitVector<T>::rank(T c, int i) {
  int count = 0;
  for (int j = 0; j < i && j < vector.size(); j++) {
    if (vector[j] == c) {
      count++;
    }
  }
  return count;
}

template <typename T> int BitVector<T>::select(T c, int i) {
  int count = 0;
  for (int j = 0; j < vector.size(); j++) {
    if (vector[j] == c) {
      count++;
    }
    if (count == i+1) {
      return j;
    }
  }

  return -1;
}

template <typename T> void BitVector<T>::pushBack(T c) { vector.push_back(c); }

template <typename T> void BitVector<T>::print() {
  for (int i = 0; i < vector.size(); i++) {
    std::cout << vector[i];
  }
  std::cout << std::endl;
}

template <typename T> std::vector<T> BitVector<T>::getVector() {
  return vector;
}

template class BitVector<bool>;
template class BitVector<int>;
