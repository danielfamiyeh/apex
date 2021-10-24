project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(files
    ../tree/Vertex.h
    ../tree/Vertex.cpp
        ../util/BitVector.h
        ../util/BitVector.cpp
        ../graph/DeBruijnGraph.h
        ../graph/DeBruijnGraph.cpp
        ../graph/PairedDeBruijnGraph.h
        ../graph/PairedDeBruijnGraph.cpp
        DeBruijnGraphTest.cpp KOrderDeBruijnGraphTest.cpp
        PairedDeBruijnGraphTest.cpp)

# adding the Google_Tests_run target
add_executable(Google_Tests_run BitVectorTest.cpp WaveletTreeTest.cpp ${files})

# linking Google_Tests_run with DateConverter_lib which will be tested
target_link_libraries(Google_Tests_run gtest gtest_main)