//
// Created by daniel on 10/10/2021.
//

#ifndef DNASSEMBLER_ISUCCINCTSTRUCTURE_H
#define DNASSEMBLER_ISUCCINCTSTRUCTURE_H

template <typename T> class ISuccinctStructure {
public:
  virtual T access(int i) = 0;
  virtual int rank(T c, int i) = 0;
  virtual int select(T c, int i) = 0;
};

#endif // DNASSEMBLER_ISUCCINCTSTRUCTURE_H