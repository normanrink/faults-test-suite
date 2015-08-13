cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)

BUILD_PLAIN_TEST(bubblesort.random veri
                 bubblesort.random.main.c bubblesort.random.enc.c
                 10 10)
BUILD_REF_OUTPUT(bubblesort.random veri "")
BUILD_ENCODED_TEST(bubblesort.random veri
                   bubblesort.random.main.c bubblesort.random.enc.c
                   10 10)

BUILD_PLAIN_TEST(bubblesort.random perf
                 bubblesort.random.main.c bubblesort.random.enc.c
                 ${LENGTH_PERF} 100)
BUILD_REF_OUTPUT(bubblesort.random perf "")
BUILD_ENCODED_TEST(bubblesort.random perf
                   bubblesort.random.main.c bubblesort.random.enc.c
                   ${LENGTH_PERF} 100)

BUILD_PLAIN_TEST(bubblesort.random cover
                 bubblesort.random.main.c bubblesort.random.enc.c
                 10 1)
BUILD_REF_OUTPUT(bubblesort.random cover "")
BUILD_ENCODED_TEST(bubblesort.random cover
                   bubblesort.random.main.c bubblesort.random.enc.c
                   10 1)
