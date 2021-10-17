project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(files
    ../tree/Vertex.h
    ../tree/Vertex.cpp
    ../succinct/BitVector.h
    ../succinct/BitVector.cpp
        ../succinct/DeBruijnGraph.h
        ../succinct/DeBruijnGraph.cpp
        DeBruijnGraphTest.cpp KOrderDeBruijnGraphTest.cpp)

# adding the Google_Tests_run target
add_executable(Google_Tests_run BitVectorTest.cpp WaveletTreeTest.cpp ${files})

# linking Google_Tests_run with DateConverter_lib which will be tested
target_link_libraries(Google_Tests_run gtest gtest_main)