cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)

BUILD_PLAIN_TEST(matmul_overflow  veri
                 matmul_overflow.main.c matmul_overflow.enc.c
                 "False" 1024 10)
BUILD_REF_OUTPUT(matmul_overflow veri "")
BUILD_ENCODED_TEST(matmul_overflow veri
                   matmul_overflow.main.c matmul_overflow.enc.c
                   "False" 1024 10)

# NOTE: This test case is only used for verification. (Perfomance and coverage analyses are
# done in the "matmul" test case.)
