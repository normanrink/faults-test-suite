cmake_minimum_required(VERSION 2.8)

include(../../cmake/functions.cmake)
include(../../cmake/flags.cmake)


set(QUICKSORT_MAIN_C   ${CMAKE_CURRENT_SOURCE_DIR}/quicksort.best.main.c)
set(QUICKSORT_MAIN_BC  quicksort.best.main.bc)
set(QUICKSORT_ENC_C    ${CMAKE_CURRENT_SOURCE_DIR}/quicksort.best.enc.c)
set(QUICKSORT_ENC_BC   quicksort.best.enc.bc)
set(INPUT_C            ${CMAKE_CURRENT_SOURCE_DIR}/quicksort.best.input.c)
set(INPUT_BC           quicksort.best.input.bc)


set(ENCODED_BC       quicksort.best.enc.encoded.bc)
set(ENCODED_LL       quicksort.best.enc.encoded.ll)
set(ENCODED_INPUT_BC quicksort.best.input.encoded.bc)
set(ENCODED_INPUT_LL quicksort.best.input.encoded.ll)


set(PLAIN_TARGET     quicksort.best.plain)
set(PLAIN_TARGET_S   quicksort.best.plain.s)
set(ENCODED_TARGET   quicksort.best.encoded)
set(ENCODED_TARGET_S quicksort.best.encoded.s)


# Build the bitcode file containing the input array:
ADD_BC_TARGET(${INPUT_BC} ${INPUT_C} "${FLAG_OPTS};${FLAG_DEBUG}")


# Build the bitcode file for the main application:
set(FLAGS ${FLAG_DEBUG} ${FLAG_OPTS}
          -I${UTILS_INC_PATH}
          -DLENGTH=${LENGTH}
          -DREPETITIONS=${REPETITIONS}
          ${PREP_DEBUG}
          ${PREP_CYCLES}
          ${PREP_CHECKSUM})
ADD_BC_TARGET(${QUICKSORT_MAIN_BC} ${QUICKSORT_MAIN_C} "${FLAGS}")


# Build the bitcode file to be encoded:
set(FLAGS ${FLAG_DEBUG} -O0 -mno-sse)
ADD_BC_TARGET(${QUICKSORT_ENC_BC} ${QUICKSORT_ENC_C} "${FLAGS}")


# Encode:
ADD_ENCODED_BC_TARGET(${ENCODED_BC} ${QUICKSORT_ENC_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_LL} ${ENCODED_BC} "")

ADD_ENCODED_BC_TARGET(${ENCODED_INPUT_BC} ${INPUT_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_INPUT_LL} ${ENCODED_INPUT_BC} "")


# Link the 'plain' application binary:
ADD_BIN_TARGET(${PLAIN_TARGET}
               "${QUICKSORT_MAIN_BC};${QUICKSORT_ENC_BC};${INPUT_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${PLAIN_TARGET_S} ${PLAIN_TARGET})


# Link the 'encoded' application binary:
ADD_BIN_TARGET(${ENCODED_TARGET}
               "${QUICKSORT_MAIN_BC};${ENCODED_BC};${ENCODED_INPUT_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${ENCODED_TARGET_S} ${ENCODED_TARGET})


# Generate reference output from the plain binary:
ADD_OUTPUT_TARGET("quicksort.best.out"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PLAIN_TARGET};--cso;quicksort.best.out"
                  ${PLAIN_TARGET})


if(BUILD_CONFIGS)
  add_subdirectory(configs)
endif(BUILD_CONFIGS)

