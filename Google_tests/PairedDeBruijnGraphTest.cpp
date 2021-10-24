//
// Created by daniel on 24/10/2021.
//

#include "../graph/PairedDeBruijnGraph.h"
#include "lib/googletest/include/gtest/gtest.h"

#include <iostream>
#include <utility>

TEST(PairedDeBruijnGraphTest, BWTMethods) {
  std::string ecoliPath = "../../genomes/ecoli/SRR16212179";

  auto *graph =
      new PairedDeBruijnGraph(ecoliPath + "_1.fastq", ecoliPath + "_2.fastq",
                              5);

  ASSERT_EQ(graph->forward(1, "forward"), 45);
  ASSERT_EQ(graph->forward(1, "reverse"), 42);

  ASSERT_EQ(graph->backward(45, "forward"), 1);
  ASSERT_EQ(graph->backward(42, "reverse"), 1);

  delete graph;
}