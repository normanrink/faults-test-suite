cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)

BUILD_PLAIN_TEST(array_sum veri array_sum.main.c array_sum.enc.c "False" 100 10)
BUILD_REF_OUTPUT(array_sum veri "")
BUILD_ENCODED_TEST(array_sum veri array_sum.main.c array_sum.enc.c "False" 100 10)

BUILD_PLAIN_TEST(array_sum perf array_sum.main.c array_sum.enc.c "False" ${LENGTH_PERF} 100)
BUILD_REF_OUTPUT(array_sum perf "")
BUILD_ENCODED_TEST(array_sum perf array_sum.main.c array_sum.enc.c "False" ${LENGTH_PERF} 100)

BUILD_PLAIN_TEST(array_sum cover array_sum.main.c array_sum.enc.c "True" 10 1)
BUILD_REF_OUTPUT(array_sum cover "")
BUILD_ENCODED_TEST(array_sum cover array_sum.main.c array_sum.enc.c "True" 10 1)
