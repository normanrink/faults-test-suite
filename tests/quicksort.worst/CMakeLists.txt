cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)

BUILD_PLAIN_TEST(quicksort.worst veri
                 quicksort.worst.main.c quicksort.worst.enc.c
                 100 10)
BUILD_REF_OUTPUT(quicksort.worst veri "")
BUILD_ENCODED_TEST(quicksort.worst veri
                   quicksort.worst.main.c quicksort.worst.enc.c
                   100 10)

BUILD_PLAIN_TEST(quicksort.worst perf
                 quicksort.worst.main.c quicksort.worst.enc.c
                 ${LENGTH_PERF} 100)
BUILD_REF_OUTPUT(quicksort.worst perf "")
BUILD_ENCODED_TEST(quicksort.worst perf
                   quicksort.worst.main.c quicksort.worst.enc.c
                   ${LENGTH_PERF} 100)

BUILD_PLAIN_TEST(quicksort.worst cover
                 quicksort.worst.main.c quicksort.worst.enc.c
                 10 1)
BUILD_REF_OUTPUT(quicksort.worst cover "")
BUILD_ENCODED_TEST(quicksort.worst cover
                   quicksort.worst.main.c quicksort.worst.enc.c
                   10 1)
