cmake_minimum_required(VERSION 3.20)
project(DNAssembler)
add_subdirectory("Google_tests")

set(CMAKE_CXX_STANDARD 14)

add_executable(DNAssembler tree/Vertex.cpp succinct/ISuccinctStructure.h succinct/WaveletTree.cpp succinct/WaveletTree.h succinct/BitVector.cpp succinct/BitVector.h succinct/DeBruijnGraph.cpp succinct/DeBruijnGraph.h main.cpp)
