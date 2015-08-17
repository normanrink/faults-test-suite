cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)

add_custom_target(crc_input_small.pcm ALL
                  COMMAND cmake -E copy
                          ${CMAKE_CURRENT_SOURCE_DIR}/crc_input_small.pcm
                          ${CMAKE_CURRENT_BINARY_DIR}/crc_input_small.pcm
                  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/crc_input_small.pcm)

BUILD_PLAIN_TEST(crc veri crc.main.c crc.enc.c "False" 1 1)
BUILD_REF_OUTPUT(crc veri crc_input_small.pcm)
BUILD_ENCODED_TEST(crc veri crc.main.c crc.enc.c "False" 1 1)

BUILD_PLAIN_TEST(crc perf crc.main.c crc.enc.c "False" 1 1)
BUILD_REF_OUTPUT(crc perf crc_input_small.pcm)
BUILD_ENCODED_TEST(crc perf crc.main.c crc.enc.c "False" 1 1)

BUILD_PLAIN_TEST(crc cover crc.main.c crc.enc.c "True" 1 1)
BUILD_REF_OUTPUT(crc cover crc_input_small.pcm)
BUILD_ENCODED_TEST(crc cover crc.main.c crc.enc.c "True" 1 1)
