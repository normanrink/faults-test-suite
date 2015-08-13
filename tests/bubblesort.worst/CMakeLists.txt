cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)

BUILD_PLAIN_TEST(bubblesort.worst veri
                 bubblesort.worst.main.c bubblesort.worst.enc.c
                 10 10)
BUILD_REF_OUTPUT(bubblesort.worst veri "")
BUILD_ENCODED_TEST(bubblesort.worst veri
                   bubblesort.worst.main.c bubblesort.worst.enc.c
                   10 10)

BUILD_PLAIN_TEST(bubblesort.worst perf
                 bubblesort.worst.main.c bubblesort.worst.enc.c
                 ${LENGTH_PERF} 100)
BUILD_REF_OUTPUT(bubblesort.worst perf "")
BUILD_ENCODED_TEST(bubblesort.worst perf
                   bubblesort.worst.main.c bubblesort.worst.enc.c
                   ${LENGTH_PERF} 100)

BUILD_PLAIN_TEST(bubblesort.worst cover
                 bubblesort.worst.main.c bubblesort.worst.enc.c
                 10 1)
BUILD_REF_OUTPUT(bubblesort.worst cover "")
BUILD_ENCODED_TEST(bubblesort.worst cover
                   bubblesort.worst.main.c bubblesort.worst.enc.c
                   10 1)

