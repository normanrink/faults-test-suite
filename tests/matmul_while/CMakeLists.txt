cmake_minimum_required(VERSION 2.8)

include(../../cmake/functions.cmake)
include(../../cmake/flags.cmake)


set(MATMUL_MAIN_C  ${CMAKE_CURRENT_SOURCE_DIR}/matmul_while.main.c)
set(MATMUL_MAIN_BC matmul_while.main.bc)
set(MATMUL_ENC_C   ${CMAKE_CURRENT_SOURCE_DIR}/matmul_while.enc.c)
set(MATMUL_ENC_BC  matmul_while.enc.bc)
set(MATRIX_C       ${CMAKE_CURRENT_SOURCE_DIR}/matmul_while.matrix.c)
set(MATRIX_BC      matmul_while.matrix.bc)
set(VECTOR_C       ${CMAKE_CURRENT_SOURCE_DIR}/matmul_while.vector.c)
set(VECTOR_BC      matmul_while.vector.bc)


set(ENCODED_BC        matmul_while.enc.encoded.bc)
set(ENCODED_LL        matmul_while.enc.encoded.ll)
set(ENCODED_MATRIX_BC matmul_while.matrix.encoded.bc)
set(ENCODED_MATRIX_LL matmul_while.matrix.encoded.ll)
set(ENCODED_VECTOR_BC matmul_while.vector.encoded.bc)
set(ENCODED_VECTOR_LL matmul_while.vector.encoded.ll)


set(PLAIN_TARGET     matmul_while.plain)
set(PLAIN_TARGET_S   matmul_while.plain.s)
set(ENCODED_TARGET   matmul_while.encoded)
set(ENCODED_TARGET_S matmul_while.encoded.s)


# Build bitcode files containing arrays of data:
ADD_BC_TARGET(${MATRIX_BC} ${MATRIX_C} "${FLAG_OPTS};${FLAG_DEBUG}")
ADD_BC_TARGET(${VECTOR_BC} ${VECTOR_C} "${FLAG_OPTS};${FLAG_DEBUG}")


# Build the bitcode file for the main application:
set(FLAGS ${FLAG_DEBUG} ${FLAG_OPTS}
          -I${UTILS_INC_PATH}
          -DMATMUL_LENGTH=${MATMUL_LENGTH}
          -DREPETITIONS=${REPETITIONS}
          ${PREP_DEBUG}
          ${PREP_CYCLES}
          ${PREP_CHECKSUM})
ADD_BC_TARGET(${MATMUL_MAIN_BC} ${MATMUL_MAIN_C} "${FLAGS}")


# Build the bitcode file to be encoded:
set(FLAGS ${FLAG_DEBUG} -O0 -mno-sse)
ADD_BC_TARGET(${MATMUL_ENC_BC} ${MATMUL_ENC_C} "${FLAGS}")


# Encode:
ADD_ENCODED_BC_TARGET(${ENCODED_BC} ${MATMUL_ENC_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_LL} ${ENCODED_BC} "")

ADD_ENCODED_BC_TARGET(${ENCODED_MATRIX_BC} ${MATRIX_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_MATRIX_LL} ${ENCODED_MATRIX_BC} "")

ADD_ENCODED_BC_TARGET(${ENCODED_VECTOR_BC} ${VECTOR_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_VECTOR_LL} ${ENCODED_VECTOR_BC} "")


# Link the 'plain' application binary:
ADD_BIN_TARGET(${PLAIN_TARGET}
               "${MATMUL_MAIN_BC};${MATMUL_ENC_BC};${MATRIX_BC};${VECTOR_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${PLAIN_TARGET_S} ${PLAIN_TARGET})


# Link the 'encoded' application binary:
ADD_BIN_TARGET(${ENCODED_TARGET}
               "${MATMUL_MAIN_BC};${ENCODED_BC};${ENCODED_MATRIX_BC};${ENCODED_VECTOR_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${ENCODED_TARGET_S} ${ENCODED_TARGET})


# Generate reference output from the plain binary:
ADD_OUTPUT_TARGET("matmul_while.out"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PLAIN_TARGET};--cso;matmul_while.out"
                  ${PLAIN_TARGET})


if(BUILD_CONFIGS)
  add_subdirectory(configs)
endif(BUILD_CONFIGS)
