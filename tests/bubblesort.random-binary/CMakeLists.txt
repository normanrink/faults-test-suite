cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)

BUILD_PLAIN_TEST(bubblesort.random-binary veri
                 bubblesort.random-binary.main.c bubblesort.random-binary.enc.c
                 "False" 10 10)
BUILD_REF_OUTPUT(bubblesort.random-binary veri "")
BUILD_ENCODED_TEST(bubblesort.random-binary veri
                   bubblesort.random-binary.main.c bubblesort.random-binary.enc.c
                   "False" 10 10)

BUILD_PLAIN_TEST(bubblesort.random-binary perf
                 bubblesort.random-binary.main.c bubblesort.random-binary.enc.c
                 "False" ${LENGTH_PERF} 100)
BUILD_REF_OUTPUT(bubblesort.random-binary perf "")
BUILD_ENCODED_TEST(bubblesort.random-binary perf
                   bubblesort.random-binary.main.c bubblesort.random-binary.enc.c
                   "False" ${LENGTH_PERF} 100)

BUILD_PLAIN_TEST(bubblesort.random-binary cover
                 bubblesort.random-binary.main.c bubblesort.random-binary.enc.c
                 "True" 10 1)
BUILD_REF_OUTPUT(bubblesort.random-binary cover "")
BUILD_ENCODED_TEST(bubblesort.random-binary cover
                   bubblesort.random-binary.main.c bubblesort.random-binary.enc.c
                   "True" 10 1)
