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

TEST(KOrderDeBruijnGraphTest, GraphMethods) {
  auto *dbg = new DeBruijnGraph(3, "../../sample-edges-2");

  // Outdegree
  for (int i = 0; i < 15; i++) {
    ASSERT_EQ(dbg->outdegree(i), ((i == 7 || i == 9) ? 2 : 1));
  }

  // Outgoing
  ASSERT_EQ(dbg->outgoing(0, "T"), 14);
  ASSERT_EQ(dbg->outgoing(1, "T"), 15);
  ASSERT_EQ(dbg->outgoing(2, "G"), 13);
  ASSERT_EQ(dbg->outgoing(3, "C"), 8);
  ASSERT_EQ(dbg->outgoing(4, "C"), 9);
  ASSERT_EQ(dbg->outgoing(5, "C"), 11);
  ASSERT_EQ(dbg->outgoing(6, "C"), 11);
  ASSERT_EQ(dbg->outgoing(7, "$1"), 1);
  ASSERT_EQ(dbg->outgoing(7, "$3"), 0);
  ASSERT_EQ(dbg->outgoing(8, "A"), 3);
  ASSERT_EQ(dbg->outgoing(9, "T"), 16);
  ASSERT_EQ(dbg->outgoing(10, "$2"), 2);
  ASSERT_EQ(dbg->outgoing(11, "A"), 4);
  //  ASSERT_EQ(dbg->outgoing(12, "A"), 5);
  //  ASSERT_EQ(dbg->outgoing(13, "A"), 6);
  ASSERT_EQ(dbg->outgoing(14, "C"), 12);

  // Label
  std::string nodeLabels[15] = {
      "$0$0$0", "$1$1$1", "$2$2$2", "ACA", "$2GA",  "$0TA",  "$1TA",
      "CAC",    "GAC",    "TAC",    "CTC", "$2$2G", "$0$0T", "$1$1T", "ACT"};

  for (int i = 0; i < 15; i++) {
    ASSERT_EQ(dbg->label(i), nodeLabels[i]);
  }
  // Indegree

  // Incoming
}