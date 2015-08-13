cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)

BUILD_PLAIN_TEST(ptrarith veri ptrarith.main.c ptrarith.enc.c 1024 10)
BUILD_REF_OUTPUT(ptrarith veri "")
BUILD_ENCODED_TEST(ptrarith veri ptrarith.main.c ptrarith.enc.c 1024 10)

BUILD_PLAIN_TEST(ptrarith perf ptrarith.main.c ptrarith.enc.c ${LENGTH_PERF} 100)
BUILD_REF_OUTPUT(ptrarith perf "")
BUILD_ENCODED_TEST(ptrarith perf ptrarith.main.c ptrarith.enc.c ${LENGTH_PERF} 100)

BUILD_PLAIN_TEST(ptrarith cover ptrarith.main.c ptrarith.enc.c 32 1)
BUILD_REF_OUTPUT(ptrarith cover "")
BUILD_ENCODED_TEST(ptrarith cover ptrarith.main.c ptrarith.enc.c 32 1)
