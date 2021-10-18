//
// Created by daniel on 17/10/2021.
//

#include "../succinct/DeBruijnGraph.h"
#include "lib/googletest/include/gtest/gtest.h"

#include <iostream>

TEST(KOrderDeBruijnGraphTest, BWTMethods) {
  auto *dbg = new DeBruijnGraph(3, "../../sample-edges-2");
  int forwardDest[17] = {14, 15, 13, 8, 9, 11, 11, 1, 0,
                         3,  3,  16, 2, 4, 5,  6,  12};

  int backwardDest[17] = {8, 7, 12, 9,  13, 14, 15, 3, 3,
                          4, 5, 5,  16, 2,  0,  1,  11};

  for (int i = 0; i < 17; i++) {
    ASSERT_EQ(dbg->forward(i), forwardDest[i]);
  }

  for (int i = 0; i < 17; i++) {
    ASSERT_EQ(dbg->backward(i), backwardDest[i]);
  }
}

TEST(KOrderDeBruijnGraphTest, GraphMethods) {}