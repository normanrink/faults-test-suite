cmake_minimum_required(VERSION 2.8)


# Variables for building the test cases:
set(ENCODER   "" CACHE PATH "")

# 'objdump' for disassembling binaries:
set(OBJDUMP   objdump)


# Variables for configuring the test cases:
set(LENGTH          "32"   CACHE STRING "")
set(REPETITIONS     "1"    CACHE STRING "")
set(REPORT_CHECKSUM FALSE CACHE BOOL   "")
set(REPORT_CYCLES   FALSE CACHE BOOL   "")


# Build the test cases:
add_subdirectory(array_sum)
add_subdirectory(crc)
add_subdirectory(des)
add_subdirectory(dijkstra)
add_subdirectory(matmul)
add_subdirectory(matmul_overflow)
add_subdirectory(matmul_while)
add_subdirectory(ptrarith)
# The sorting algorithms are built with with different inputs:
add_subdirectory(bubblesort.best)
add_subdirectory(bubblesort.random)
add_subdirectory(bubblesort.random-binary)
#add_subdirectory(bubblesort.worst)
add_subdirectory(quicksort.best)
add_subdirectory(quicksort.random)
add_subdirectory(quicksort.random-binary)
#add_subdirectory(quicksort.worst)
