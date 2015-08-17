cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)

configure_file(dijkstra_input.dat dijkstra_input.dat)

BUILD_PLAIN_TEST(dijkstra veri dijkstra.main.c dijkstra.enc.c "False" 1 1)
BUILD_REF_OUTPUT(dijkstra veri dijkstra_input.dat)
BUILD_ENCODED_TEST(dijkstra veri dijkstra.main.c dijkstra.enc.c "False" 1 1)

BUILD_PLAIN_TEST(dijkstra perf dijkstra.main.c dijkstra.enc.c "False" 1 1)
BUILD_REF_OUTPUT(dijkstra perf dijkstra_input.dat)
BUILD_ENCODED_TEST(dijkstra perf dijkstra.main.c dijkstra.enc.c "False" 1 1)

BUILD_PLAIN_TEST(dijkstra cover dijkstra.main.c dijkstra.enc.c "True" 1 1)
BUILD_REF_OUTPUT(dijkstra cover dijkstra_input.dat)
BUILD_ENCODED_TEST(dijkstra cover dijkstra.main.c dijkstra.enc.c "True" 1 1)
