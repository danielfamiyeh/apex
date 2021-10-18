//
// Created by daniel on 15/10/2021.
//

#include "../succinct/DeBruijnGraph.h"
#include "lib/googletest/include/gtest/gtest.h"

#include <iostream>

TEST(DeBruijnGraphTest, BWTMethods) {
  auto *dbg = new DeBruijnGraph(3, "../../sample-edges");
  int dest[13]{10, 4, 5, 8, 11, 8, 9, 1, 12, 1, 2, 0, 6};
  int dest2[13]{11, 7, 10, 1, 1, 2, 12, 3, 3, 6, 0, 4, 8};

  for (int i = 0; i < 13; i++) {
    ASSERT_EQ(dbg->forward(i), dest[i]);
  }

  for (int i = 0; i < 13; i++) {
    ASSERT_EQ(dbg->backward(i), dest2[i]);
  }
}

TEST(DeBruijnGraphTest, GraphMethods) {
  auto *dbg = new DeBruijnGraph(3, "../../sample-edges");

  // Outdegree
  ASSERT_EQ(dbg->outdegree(3), 2);
  ASSERT_EQ(dbg->outdegree(6), 2);
  ASSERT_EQ(dbg->outdegree(1), 1);

  // Outgoing
  ASSERT_EQ(dbg->outgoing(6, "T"), 12);
  ASSERT_EQ(dbg->outgoing(2, "C"), 5);
  ASSERT_EQ(dbg->outgoing(6, "G"), -1);

  // Label
  std::string labels[11] = {
      std::string("$0$0$0"), std::string("CGA"), std::string("$0TA"),
      std::string("GAC"),    std::string("TAC"), std::string("GTC"),
      std::string("ACG"),    std::string("TCG"), std::string("$0$0T"),
      std::string("ACT"),    std::string("CGT")};

  for (int i = 0; i < 11; i++) {
    ASSERT_EQ(dbg->label(i), labels[i]);
  }

  // Indegree
  int indegrees[11] = {0, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1};
  for (int i = 0; i < 11; i++) {
    ASSERT_EQ(dbg->indegree(i), indegrees[i]);
  }

  // Incoming
  ASSERT_EQ(dbg->incoming(0, "T"), -1);
  ASSERT_EQ(dbg->incoming(1, "A"), 6);
  ASSERT_EQ(dbg->incoming(2, "$0"), 8);
  ASSERT_EQ(dbg->incoming(3, "C"), 1);
  ASSERT_EQ(dbg->incoming(4, "$"), 2);
  ASSERT_EQ(dbg->incoming(5, "C"), 10);
  ASSERT_EQ(dbg->incoming(6, "G"), 3);
  ASSERT_EQ(dbg->incoming(7, "G"), 5);
  ASSERT_EQ(dbg->incoming(8, "$"), 0);
  ASSERT_EQ(dbg->incoming(9, "G"), 3);
  ASSERT_EQ(dbg->incoming(10, "A"), 6);
}