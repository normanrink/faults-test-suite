cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)

BUILD_PLAIN_TEST(quicksort.random veri
                 quicksort.random.main.c quicksort.random.enc.c
                 100 10)
BUILD_REF_OUTPUT(quicksort.random veri "")
BUILD_ENCODED_TEST(quicksort.random veri
                   quicksort.random.main.c quicksort.random.enc.c
                   100 10)

BUILD_PLAIN_TEST(quicksort.random perf
                 quicksort.random.main.c quicksort.random.enc.c
                 ${LENGTH_PERF} 100)
BUILD_REF_OUTPUT(quicksort.random perf "")
BUILD_ENCODED_TEST(quicksort.random perf
                   quicksort.random.main.c quicksort.random.enc.c
                   ${LENGTH_PERF} 100)

BUILD_PLAIN_TEST(quicksort.random cover
                 quicksort.random.main.c quicksort.random.enc.c
                 10 1)
BUILD_REF_OUTPUT(quicksort.random cover "")
BUILD_ENCODED_TEST(quicksort.random cover
                   quicksort.random.main.c quicksort.random.enc.c
                   10 1)
