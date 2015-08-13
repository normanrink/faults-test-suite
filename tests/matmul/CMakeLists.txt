cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)

BUILD_PLAIN_TEST(matmul veri matmul.main.c matmul.enc.c 1024 10)
BUILD_REF_OUTPUT(matmul veri "")
BUILD_ENCODED_TEST(matmul veri matmul.main.c matmul.enc.c 1024 10)

BUILD_PLAIN_TEST(matmul perf matmul.main.c matmul.enc.c ${LENGTH_PERF} 100)
BUILD_REF_OUTPUT(matmul perf matmul_input.dat)
BUILD_ENCODED_TEST(matmul perf matmul.main.c matmul.enc.c ${LENGTH_PERF} 100)

BUILD_PLAIN_TEST(matmul cover matmul.main.c matmul.enc.c 64 1)
BUILD_REF_OUTPUT(matmul cover matmul_input.dat)
BUILD_ENCODED_TEST(matmul cover matmul.main.c matmul.enc.c 64 1)
