cmake_minimum_required(VERSION 3.15)
project(Grafy_Dijkstra_BellmanFord CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

find_package(OpenMP REQUIRED)

include_directories(include tests)

set(CORE_SOURCES
    src/algorithms.cpp
    src/graph.cpp
    src/min_heap.cpp
    tests/tests.cpp 
)

add_executable(program_glowny src/main.cpp ${CORE_SOURCES})
target_link_libraries(program_glowny PRIVATE OpenMP::OpenMP_CXX)
target_compile_options(program_glowny PRIVATE -O2)

add_executable(sprawdzarka src/comparator.cpp)
target_compile_options(sprawdzarka PRIVATE -O2)