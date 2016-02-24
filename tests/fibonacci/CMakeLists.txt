cmake_minimum_required(VERSION 2.8)

include(../../cmake/functions.cmake)
include(../../cmake/flags.cmake)


set(FIBONACCI_LENGTH   "8"   CACHE STRING "")


set(FIBONACCI_MAIN_C  ${CMAKE_CURRENT_SOURCE_DIR}/fibonacci.main.c)
set(FIBONACCI_MAIN_BC fibonacci.main.bc)
set(FIBONACCI_ENC_C   ${CMAKE_CURRENT_SOURCE_DIR}/fibonacci.enc.c)
set(FIBONACCI_ENC_BC  fibonacci.enc.bc)


set(ENCODED_BC       fibonacci.enc.encoded.bc)
set(ENCODED_LL       fibonacci.enc.encoded.ll)


set(PLAIN_TARGET     fibonacci.plain)
set(PLAIN_TARGET_S   fibonacci.plain.s)
set(ENCODED_TARGET   fibonacci.encoded)
set(ENCODED_TARGET_S fibonacci.encoded.s)


# Build the bitcode file for the main application:
set(FLAGS ${FLAG_DEBUG} ${FLAG_OPTS}
          -I${UTILS_INC_PATH}
          -DFIBONACCI_LENGTH=${FIBONACCI_LENGTH}
          -DREPETITIONS=${REPETITIONS}
          ${PREP_DEBUG}
          ${PREP_CYCLES}
          ${PREP_CHECKSUM})
ADD_BC_TARGET(${FIBONACCI_MAIN_BC} ${FIBONACCI_MAIN_C} "${FLAGS}")


# Build the bitcode file to be encoded:
set(FLAGS ${FLAG_DEBUG} -O0 -mno-sse)
ADD_BC_TARGET(${FIBONACCI_ENC_BC} ${FIBONACCI_ENC_C} "${FLAGS}")


# Encode:
ADD_ENCODED_BC_TARGET(${ENCODED_BC} ${FIBONACCI_ENC_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_LL} ${ENCODED_BC} "")


# Link the 'plain' application binary:
ADD_BIN_TARGET(${PLAIN_TARGET}
               "${FIBONACCI_MAIN_BC};${FIBONACCI_ENC_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${PLAIN_TARGET_S} ${PLAIN_TARGET})


# Link the 'encoded' application binary:
ADD_BIN_TARGET(${ENCODED_TARGET}
               "${FIBONACCI_MAIN_BC};${ENCODED_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${ENCODED_TARGET_S} ${ENCODED_TARGET})


# Generate reference output from the plain binary:
ADD_OUTPUT_TARGET("fibonacci.out"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PLAIN_TARGET};--cso;fibonacci.out"
                  ${PLAIN_TARGET})


if(BUILD_CONFIGS)
  add_subdirectory(configs)
endif(BUILD_CONFIGS)
