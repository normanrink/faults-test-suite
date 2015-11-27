cmake_minimum_required(VERSION 2.8)

include(../../cmake/functions.cmake)
include(../../cmake/flags.cmake)


set(BUBBLESORT_MAIN_C  ${CMAKE_CURRENT_SOURCE_DIR}/bubblesort.random-binary.main.c)
set(BUBBLESORT_MAIN_BC bubblesort.random-binary.main.bc)
set(BUBBLESORT_ENC_C   ${CMAKE_CURRENT_SOURCE_DIR}/bubblesort.random-binary.enc.c)
set(BUBBLESORT_ENC_BC  bubblesort.random-binary.enc.bc)
set(INPUT_C            ${CMAKE_CURRENT_SOURCE_DIR}/bubblesort.random-binary.input.c)
set(INPUT_BC           bubblesort.random-binary.input.bc)


set(ENCODED_BC       bubblesort.random-binary.enc.encoded.bc)
set(ENCODED_LL       bubblesort.random-binary.enc.encoded.ll)
set(ENCODED_INPUT_BC bubblesort.random-binary.input.encoded.bc)
set(ENCODED_INPUT_LL bubblesort.random-binary.input.encoded.ll)


set(PLAIN_TARGET     bubblesort.random-binary.plain)
set(PLAIN_TARGET_S   bubblesort.random-binary.plain.s)
set(ENCODED_TARGET   bubblesort.random-binary.encoded)
set(ENCODED_TARGET_S bubblesort.random-binary.encoded.s)


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
ADD_BC_TARGET(${BUBBLESORT_MAIN_BC} ${BUBBLESORT_MAIN_C} "${FLAGS}")


# Build the bitcode file to be encoded:
set(FLAGS ${FLAG_DEBUG} -O0 -mno-sse)
ADD_BC_TARGET(${BUBBLESORT_ENC_BC} ${BUBBLESORT_ENC_C} "${FLAGS}")


# Encode:
ADD_ENCODED_BC_TARGET(${ENCODED_BC} ${BUBBLESORT_ENC_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_LL} ${ENCODED_BC} "")

ADD_ENCODED_BC_TARGET(${ENCODED_INPUT_BC} ${INPUT_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_INPUT_LL} ${ENCODED_INPUT_BC} "")


# Link the 'plain' application binary:
ADD_BIN_TARGET(${PLAIN_TARGET}
               "${BUBBLESORT_MAIN_BC};${BUBBLESORT_ENC_BC};${INPUT_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${PLAIN_TARGET_S} ${PLAIN_TARGET})


# Link the 'encoded' application binary:
ADD_BIN_TARGET(${ENCODED_TARGET}
               "${BUBBLESORT_MAIN_BC};${ENCODED_BC};${ENCODED_INPUT_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${ENCODED_TARGET_S} ${ENCODED_TARGET})

