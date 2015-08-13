cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)

BUILD_PLAIN_TEST(quicksort.best veri
                 quicksort.best.main.c quicksort.best.enc.c
                 100 10)
BUILD_REF_OUTPUT(quicksort.best veri "")
BUILD_ENCODED_TEST(quicksort.best veri
                   quicksort.best.main.c quicksort.best.enc.c
                   100 10)

BUILD_PLAIN_TEST(quicksort.best perf
                 quicksort.best.main.c quicksort.best.enc.c
                 ${LENGTH_PERF} 100)
BUILD_REF_OUTPUT(quicksort.best perf "")
BUILD_ENCODED_TEST(quicksort.best perf
                   quicksort.best.main.c quicksort.best.enc.c
                   ${LENGTH_PERF} 100)

BUILD_PLAIN_TEST(quicksort.best cover
                 quicksort.best.main.c quicksort.best.enc.c
                 10 1)
BUILD_REF_OUTPUT(quicksort.best cover "")
BUILD_ENCODED_TEST(quicksort.best cover
                   quicksort.best.main.c quicksort.best.enc.c
                   10 1)
