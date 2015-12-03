cmake_minimum_required(VERSION 2.8)

include(../../cmake/functions.cmake)
include(../../cmake/flags.cmake)


set(DES_INPUT_SIZE "tiny" CACHE STRING "<tiny|small|large|huge>")


set(DES_MAIN_C    ${CMAKE_CURRENT_SOURCE_DIR}/des.main.c)
set(DES_MAIN_BC   des.main.bc)
set(DES_ENC_C     ${CMAKE_CURRENT_SOURCE_DIR}/des.enc.c)
set(DES_ENC_BC    des.enc.bc)
set(INPUT_C       ${CMAKE_CURRENT_SOURCE_DIR}/input.${DES_INPUT_SIZE}.c)
set(INPUT_BC      input.${DES_INPUT_SIZE}.bc)


set(ENCODED_BC    des.enc.encoded.bc)
set(ENCODED_LL    des.enc.encoded.ll)


set(PLAIN_TARGET     des.plain)
set(PLAIN_TARGET_S   des.plain.s)
set(ENCODED_TARGET   des.encoded)
set(ENCODED_TARGET_S des.encoded.s)


if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  set(OPTS -O0 -g)
  set(DBG -g)
else()
  set(OPTS -O3)
endif(CMAKE_BUILD_TYPE MATCHES DEBUG)


# Build bitcode file containing array of input data:
ADD_BC_TARGET(${INPUT_BC} ${INPUT_C} "${OPTS}")


# Build the bitcode file for the main application:
set(FLAGS ${FLAG_DEBUG} ${FLAG_OPTS}
          -I${UTILS_INC_PATH}
          -DLENGTH=${LENGTH}
          -DREPETITIONS=${REPETITIONS}
          -DNUM_NODES=${DIJKSTRA_NUM_NODES}
          ${PREP_DEBUG}
          ${PREP_CYCLES}
          ${PREP_CHECKSUM})
ADD_BC_TARGET(${DES_MAIN_BC} ${DES_MAIN_C} "${FLAGS}")


# Build the bitcode file to be encoded:
set(FLAGS ${FLAG_DEBUG} -O0 -mno-sse)
ADD_BC_TARGET(${DES_ENC_BC} ${DES_ENC_C} "${FLAGS}")


# Encode:
ADD_ENCODED_BC_TARGET(${ENCODED_BC} ${DES_ENC_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_LL} ${ENCODED_BC} "")
# Note that 'input.*.c' is NOT encoded!


# Link the 'plain' application binary:
ADD_BIN_TARGET(${PLAIN_TARGET}
               "${DES_MAIN_BC};${DES_ENC_BC};${INPUT_BC}"
               "${OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${PLAIN_TARGET_S} ${PLAIN_TARGET})


# Link the 'encoded' application binary:
ADD_BIN_TARGET(${ENCODED_TARGET}
               "${DES_MAIN_BC};${ENCODED_BC};${INPUT_BC}"
               "${OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${ENCODED_TARGET_S} ${ENCODED_TARGET})


# Generate reference output from the plain binary:
ADD_OUTPUT_TARGET("des.out"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PLAIN_TARGET};--cso;des.out"
                  ${PLAIN_TARGET})


if(BUILD_CONFIGS)
  add_subdirectory(configs)
endif(BUILD_CONFIGS)
