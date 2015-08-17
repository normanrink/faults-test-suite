cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)

BUILD_PLAIN_TEST(bubblesort.best veri
                 bubblesort.best.main.c bubblesort.best.enc.c
                 "False" 10 10)
BUILD_REF_OUTPUT(bubblesort.best veri "")
BUILD_ENCODED_TEST(bubblesort.best veri
                   bubblesort.best.main.c bubblesort.best.enc.c
                   "False" 10 10)

BUILD_PLAIN_TEST(bubblesort.best perf
                 bubblesort.best.main.c bubblesort.best.enc.c
                 "False" ${LENGTH_PERF} 100)
BUILD_REF_OUTPUT(bubblesort.best perf "")
BUILD_ENCODED_TEST(bubblesort.best perf
                   bubblesort.best.main.c bubblesort.best.enc.c
                   "False" ${LENGTH_PERF} 100)

BUILD_PLAIN_TEST(bubblesort.best cover
                 bubblesort.best.main.c bubblesort.best.enc.c
                 "True" 10 1)
BUILD_REF_OUTPUT(bubblesort.best cover "")
BUILD_ENCODED_TEST(bubblesort.best cover
                   bubblesort.best.main.c bubblesort.best.enc.c
                   "True" 10 1)
