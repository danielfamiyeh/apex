//
// Created by daniel on 15/10/2021.
//

#include "lib/googletest/include/gtest/gtest.h"
#include "../succinct/DeBruijnGraph.h"

#include <iostream>

TEST (DeBruijnGraphTest, BWTMethods) {
  auto* dbg = new DeBruijnGraph(3, "../../sample-edges");
  int dest[13] {10, 4, 5, 8, 11, 8, 9, 1, 12, 1, 2, 0, 6};
  int dest2[4] {12, 7, 10,  1};


  for(int i=0; i<13; i++) {
    ASSERT_EQ(dbg->forward(i), dest[i]);
  }

//  for(int i=0; i<4; i++) {
//    ASSERT_EQ(dbg->backward(i), dest2[i]);
//  }

//  ASSERT_EQ(dbg->backward(1), 7);
//  ASSERT_EQ(dbg->backward(5), 2);
//  ASSERT_EQ(dbg->backward(4), 1);
  ASSERT_EQ(dbg->backward(0), 11);
}

TEST (DeBruijnGraphTest, GraphMethods) {
  auto* dbg = new DeBruijnGraph(3, "../../sample-edges");

  // Outdegree
//  ASSERT_EQ(dbg->outdegree(4), 2);
//  ASSERT_EQ(dbg->outdegree(7), 2);
//

}