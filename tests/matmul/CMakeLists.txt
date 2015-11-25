cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test-2.cmake)


set(MATMUL_MAIN_C  ${CMAKE_CURRENT_SOURCE_DIR}/matmul.main.c)
set(MATMUL_MAIN_BC matmul.main.bc)
set(MATMUL_ENC_C   ${CMAKE_CURRENT_SOURCE_DIR}/matmul.enc.c)
set(MATMUL_ENC_BC  matmul.enc.bc)
set(MATRIX_C       ${CMAKE_CURRENT_SOURCE_DIR}/matrix.c)
set(MATRIX_BC      matrix.bc)
set(VECTOR_C       ${CMAKE_CURRENT_SOURCE_DIR}/vector.c)
set(VECTOR_BC      vector.bc)


set(ENCODED_BC matmul.enc.encoded.bc)
set(ENCODED_LL matmul.enc.encoded.ll)
set(ENCODED_MATRIX_BC matrix.encoded.bc)
set(ENCODED_MATRIX_LL matrix.encoded.ll)
set(ENCODED_VECTOR_BC vector.encoded.bc)
set(ENCODED_VECTOR_LL vector.encoded.ll)


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


# Build bitcode files containing arrays of data:
ADD_BC_COMMAND(${MATRIX_BC} ${MATRIX_C} "${OPTS}")
ADD_BC_COMMAND(${VECTOR_BC} ${VECTOR_C} "${OPTS}")


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

ADD_ENCODED_BC_COMMAND(${ENCODED_MATRIX_BC} ${MATRIX_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_MATRIX_LL} ${ENCODED_MATRIX_BC} "")

ADD_ENCODED_BC_COMMAND(${ENCODED_VECTOR_BC} ${VECTOR_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_VECTOR_LL} ${ENCODED_VECTOR_BC} "")


# Link the 'plain' application binary:
ADD_BIN_TARGET(${PLAIN_TARGET}
               "${MATMUL_MAIN_BC};${MATMUL_ENC_BC};${MATRIX_BC};${VECTOR_BC}"
               "${OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${PLAIN_TARGET_S} ${PLAIN_TARGET})


# Link the 'encoded' application binary:
ADD_BIN_TARGET(${ENCODED_TARGET}
               "${MATMUL_MAIN_BC};${ENCODED_BC};${ENCODED_MATRIX_BC};${ENCODED_VECTOR_BC}"
               "${OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${ENCODED_TARGET_S} ${ENCODED_TARGET})


# Generate reference output from the plain binary:
ADD_OUTPUT_TARGET("matmul.out"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PLAIN_TARGET};--cso;matmul.out"
                  ${PLAIN_TARGET})

