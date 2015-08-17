cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)


BUILD_PLAIN_TEST(quicksort.random-binary veri
                 quicksort.random-binary.main.c quicksort.random-binary.enc.c
                 "False" 100 10)
BUILD_REF_OUTPUT(quicksort.random-binary veri "")
BUILD_ENCODED_TEST(quicksort.random-binary veri
                   quicksort.random-binary.main.c quicksort.random-binary.enc.c
                   "False" 100 10)

BUILD_PLAIN_TEST(quicksort.random-binary perf
                 quicksort.random-binary.main.c quicksort.random-binary.enc.c
                 "False" ${LENGTH_PERF} 100)
BUILD_REF_OUTPUT(quicksort.random-binary perf "")
BUILD_ENCODED_TEST(quicksort.random-binary perf
                   quicksort.random-binary.main.c quicksort.random-binary.enc.c
                   "False" ${LENGTH_PERF} 100)

BUILD_PLAIN_TEST(quicksort.random-binary cover
                 quicksort.random-binary.main.c quicksort.random-binary.enc.c
                 "True" 10 1)
BUILD_REF_OUTPUT(quicksort.random-binary cover "")
BUILD_ENCODED_TEST(quicksort.random-binary cover
                   quicksort.random-binary.main.c quicksort.random-binary.enc.c
                   "True" 10 1)
