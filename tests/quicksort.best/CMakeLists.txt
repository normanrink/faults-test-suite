cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)

BUILD_PLAIN_TEST(quicksort.best veri
                 quicksort.best.main.c quicksort.best.enc.c
                 "False" 100 10)
BUILD_REF_OUTPUT(quicksort.best veri "")
BUILD_ENCODED_TEST(quicksort.best veri
                   quicksort.best.main.c quicksort.best.enc.c
                   "False" 100 10)

BUILD_PLAIN_TEST(quicksort.best perf
                 quicksort.best.main.c quicksort.best.enc.c
                 "False" ${LENGTH_PERF} 100)
BUILD_REF_OUTPUT(quicksort.best perf "")
BUILD_ENCODED_TEST(quicksort.best perf
                   quicksort.best.main.c quicksort.best.enc.c
                   "False" ${LENGTH_PERF} 100)

BUILD_PLAIN_TEST(quicksort.best cover
                 quicksort.best.main.c quicksort.best.enc.c
                 "True" 10 1)
BUILD_REF_OUTPUT(quicksort.best cover "")
BUILD_ENCODED_TEST(quicksort.best cover
                   quicksort.best.main.c quicksort.best.enc.c
                   "TRue" 10 1)
