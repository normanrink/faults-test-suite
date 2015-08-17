cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test.cmake)

# Tiny test case:
configure_file(des.input.tiny.txt des.input.tiny.txt)

BUILD_PLAIN_TEST(des tiny.veri des.main.c des.enc.c "False" 1 1)
BUILD_REF_OUTPUT(des tiny.veri
                 "-e;${CMAKE_CURRENT_BINARY_DIR}/des.input.tiny.txt;/dev/null;809")
BUILD_ENCODED_TEST(des tiny.veri des.main.c des.enc.c "False" 1 1)

BUILD_PLAIN_TEST(des tiny.perf des.main.c des.enc.c "False" 1 1)
BUILD_REF_OUTPUT(des tiny.perf
                 "-e;${CMAKE_CURRENT_BINARY_DIR}/des.input.tiny.txt;/dev/null;809")
BUILD_ENCODED_TEST(des tiny.perf des.main.c des.enc.c "False" 1 1)

BUILD_PLAIN_TEST(des tiny.cover des.main.c des.enc.c "True" 1 1)
BUILD_REF_OUTPUT(des tiny.cover
                 "-e;${CMAKE_CURRENT_BINARY_DIR}/des.input.tiny.txt;/dev/null;809")
BUILD_ENCODED_TEST(des tiny.cover des.main.c des.enc.c "True" 1 1)

# Large test case:
configure_file(des.input.large.txt des.input.large.txt)

BUILD_PLAIN_TEST(des large.veri des.main.c des.enc.c "False" 1 1)
BUILD_REF_OUTPUT(des large.veri
                 "-e;${CMAKE_CURRENT_BINARY_DIR}/des.input.large.txt;/dev/null;809")
BUILD_ENCODED_TEST(des large.veri des.main.c des.enc.c "False" 1 1)

BUILD_PLAIN_TEST(des large.perf des.main.c des.enc.c "False" 1 1)
BUILD_REF_OUTPUT(des large.perf
                 "-e;${CMAKE_CURRENT_BINARY_DIR}/des.input.large.txt;/dev/null;809")
BUILD_ENCODED_TEST(des large.perf des.main.c des.enc.c "False" 1 1)

# NOTE: Only the "tiny" test case is used for coverage.

# Huge test case:
configure_file(des.input.huge.txt des.input.huge.txt)

BUILD_PLAIN_TEST(des huge.veri des.main.c des.enc.c "False" 1 1)
BUILD_REF_OUTPUT(des huge.veri
                 "-e;${CMAKE_CURRENT_BINARY_DIR}/des.input.huge.txt;/dev/null;809")
BUILD_ENCODED_TEST(des huge.veri des.main.c des.enc.c "False" 1 1)

BUILD_PLAIN_TEST(des huge.perf des.main.c des.enc.c "False" 1 1)
BUILD_REF_OUTPUT(des huge.perf
                 "-e;${CMAKE_CURRENT_BINARY_DIR}/des.input.huge.txt;/dev/null;809")
BUILD_ENCODED_TEST(des.huge perf des.main.c des.enc.c "False" 1 1)

# NOTE: Only the "tiny" test case is used for coverage.
