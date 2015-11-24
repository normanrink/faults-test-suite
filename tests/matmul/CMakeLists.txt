cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test-2.cmake)


set(MATMUL_MAIN_C  ${CMAKE_CURRENT_SOURCE_DIR}/matmul.main.c)
set(MATMUL_MAIN_BC matmul.main.bc)
set(MATMUL_ENC_C   ${CMAKE_CURRENT_SOURCE_DIR}/matmul.enc.c)
set(MATMUL_ENC_BC  matmul.enc.bc)


set(ENCODED_BC matmul.enc.encoded.bc)
set(ENCODED_LL matmul.enc.encoded.ll)


set(PLAIN_TARGET     matmul.plain)
set(PLAIN_TARGET_S   matmul.plain.s)
set(ENCODED_TARGET   matmul.encoded)
set(ENCODED_TARGET_S matmul.encoded.s)


if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  set(OPTS -g -O0)
  set(DBG -g)
else()
  set(OPTS -O3)
endif(CMAKE_BUILD_TYPE MATCHES DEBUG)


# Build the bitcode file for the main application:
set(FLAGS ${OPTS} -I${UTILS_INC_PATH} -DLENGTH=${LENGTH}
                                      -DREPETITIONS=${REPETITIONS})
ADD_BC_COMMAND(${MATMUL_MAIN_BC} ${MATMUL_MAIN_C} "${FLAGS}")


# Build the bitcode file to be encoded:
set(FLAGS -I${UTILS_INC_PATH} ${DBG} -O0 -mno-sse)
ADD_BC_COMMAND(${MATMUL_ENC_BC} ${MATMUL_ENC_C} "${FLAGS}")


# Encode:
ADD_ENCODED_BC_COMMAND(${ENCODED_BC} ${MATMUL_ENC_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_LL} ${ENCODED_BC} "")


# Link the 'plain' application binary:
ADD_BIN_TARGET(${PLAIN_TARGET}
               "${MATMUL_MAIN_BC};${MATMUL_ENC_BC}"
               "${OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${PLAIN_TARGET_S} ${PLAIN_TARGET})


# Link the 'encoded' application binary:
ADD_BIN_TARGET(${ENCODED_TARGET}
               "${MATMUL_MAIN_BC};${ENCODED_BC}"
               "${OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${ENCODED_TARGET_S} ${ENCODED_TARGET})

