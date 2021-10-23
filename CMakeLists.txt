cmake_minimum_required(VERSION 3.20)
project(DNAssembler)
add_subdirectory("Google_tests")
enable_testing()
set(CMAKE_CXX_STANDARD 20)

add_executable(DNAssembler tree/Vertex.cpp util/ISuccinctStructure.h tree/WaveletTree.cpp tree/WaveletTree.h util/BitVector.cpp util/BitVector.h graph/DeBruijnGraph.cpp graph/DeBruijnGraph.h graph/PairedDeBruijnGraph.cpp graph/PairedDeBruijnGraph.h)
