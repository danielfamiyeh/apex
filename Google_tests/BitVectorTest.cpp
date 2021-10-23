//
// Created by daniel on 15/10/2021.
//

#include "../util/BitVector.h"
#include "lib/googletest/include/gtest/gtest.h"

TEST (BitVectorTest, RankTest) {
  bool v1[6] = {true, false, true, true, false, true};
  bool v2[8] = {true, true, true, false, true, false, true, true};
  bool v3[8] = {false, false, true, false, true, false, true, true};

  auto* b1 = new BitVector<bool>(v1, 6);
  auto* b2 = new BitVector<bool>(v2, 8);
  auto* b3 = new BitVector<bool>(v3, 8);

  ASSERT_EQ(b1->rank(true, 3), 2);
  ASSERT_EQ(b1->rank(false, 5), 2);

  ASSERT_EQ(b2->rank(true, 5), 4);
  ASSERT_EQ(b2->rank(false, 3), 0);

  ASSERT_EQ(b3->rank(true, 7), 3);
  ASSERT_EQ(b3->rank(false, 4), 3);

  delete b1;
  delete b2;
  delete b3;
}

TEST (BitVectorTest, SelectTest) {
  bool v1[6] = {true, false, true, true, false, true};
  bool v2[8] = {true, true, true, false, true, false, true, true};
  bool v3[8] = {false, false, true, false, true, false, true, true};

  auto* b1 = new BitVector<bool>(v1, 6);
  auto* b2 = new BitVector<bool>(v2, 8);
  auto* b3 = new BitVector<bool>(v3, 8);

  ASSERT_EQ(b1->select(true, 3), 5);
  ASSERT_EQ(b1->select(false, 1), 4);

  ASSERT_EQ(b2->select(true, 5), 7);
  ASSERT_EQ(b2->select(false, 2), -1);

  ASSERT_EQ(b3->select(true, 2), 6);
  ASSERT_EQ(b3->select(false, 3), 5);

  delete b1;
  delete b2;
  delete b3;
}

TEST (BitVector, AccessTest) {
  bool v1[6] = {true, false, true, true, false, true};
  bool v2[8] = {true, true, true, false, true, false, true, true};
  bool v3[8] = {false, false, true, false, true, false, true, true};

  auto* b1 = new BitVector<bool>(v1, 6);
  auto* b2 = new BitVector<bool>(v2, 8);
  auto* b3 = new BitVector<bool>(v3, 8);

  ASSERT_EQ(b1->access(3), 1);
  ASSERT_EQ(b1->access(1), 0);

  ASSERT_EQ(b2->access(5), 0);
  ASSERT_EQ(b2->access(2), 1);

  ASSERT_EQ(b3->access(4), 1);
  ASSERT_EQ(b3->access(7), 1);

  delete b1;
  delete b2;
  delete b3;
}