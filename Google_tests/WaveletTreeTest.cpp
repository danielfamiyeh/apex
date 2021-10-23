//
// Created by daniel on 15/10/2021.
//

#include <vector>

#include "../tree/WaveletTree.cpp"
#include "lib/googletest/include/gtest/gtest.h"

TEST(WaveletTreeTest, AccessTest) {

  std::vector<std::string> alpha1{"A", "C", "H", "N", "T", "W", "Y"};
  std::vector<std::string> s1 = {"W", "H", "Y", "C", "A", "N", "T"};
  auto *tree1 = new WaveletTree(alpha1, s1);

  std::vector<std::string> alpha2{"H", "O", "T",
                                  "U"
                                  "Y"};
  std::vector<std::string> s2 = {"Y", "O", "U", "T", "H"};
  auto *tree2 = new WaveletTree(alpha2, s2);

  std::vector<std::string> alpha3{"A", "F", "I", "K", "N", "O"};
  std::vector<std::string> s3 = {"I", "N", "K", "O", "F", "A"};
  auto *tree3 = new WaveletTree(alpha3, s3);

  ASSERT_EQ(tree1->access(4), "A");
  ASSERT_EQ(tree1->access(0), "W");
  ASSERT_EQ(tree1->access(6), "T");

  ASSERT_EQ(tree2->access(4), "H");
  ASSERT_EQ(tree2->access(3), "T");

  ASSERT_EQ(tree3->access(4), "F");
  ASSERT_EQ(tree3->access(2), "K");
  ASSERT_EQ(tree3->access(1), "N");

  delete tree1;
  delete tree2;
  delete tree3;
}

TEST(WaveletTreeTest, RankTest) {
  std::vector<std::string> alpha1{"M", "I", "S", "P"};
  std::vector<std::string> s1 = {"M", "I", "S", "S", "I", "S",
                                 "S", "I", "P", "P", "I"};

  auto *tree1 = new WaveletTree(alpha1, s1);

  ASSERT_EQ(tree1->rank("I", 6), 2);
  ASSERT_EQ(tree1->rank("S", 6), 3);
  ASSERT_EQ(tree1->rank("I", 7), 2);
  ASSERT_EQ(tree1->rank("P", 10), 2);

  delete tree1;
}

TEST(WaveletTreeTest, SelectTest) {
  std::vector<std::string> alpha1{"M", "I", "S", "P"};
  std::vector<std::string> s1 = {"M", "I", "S", "S", "I", "S",
                                 "S", "I", "P", "P", "I"};

  auto *tree1 = new WaveletTree(alpha1, s1);

  ASSERT_EQ(tree1->select("S", 3), 6);
  ASSERT_EQ(tree1->select("I", 3), 10);
  ASSERT_EQ(tree1->select("S", 1), 3);

  delete tree1;
}