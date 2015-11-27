cmake_minimum_required(VERSION 2.8)

include(../../cmake/functions.cmake)
include(../../cmake/flags.cmake)


set(QUICKSORT_MAIN_C  ${CMAKE_CURRENT_SOURCE_DIR}/quicksort.random.main.c)
set(QUICKSORT_MAIN_BC quicksort.random.main.bc)
set(QUICKSORT_ENC_C   ${CMAKE_CURRENT_SOURCE_DIR}/quicksort.random.enc.c)
set(QUICKSORT_ENC_BC  quicksort.random.enc.bc)
set(INPUT_C            ${CMAKE_CURRENT_SOURCE_DIR}/quicksort.random.input.c)
set(INPUT_BC           quicksort.random.input.bc)


set(ENCODED_BC       quicksort.random.enc.encoded.bc)
set(ENCODED_LL       quicksort.random.enc.encoded.ll)
set(ENCODED_INPUT_BC quicksort.random.input.encoded.bc)
set(ENCODED_INPUT_LL quicksort.random.input.encoded.ll)


set(PLAIN_TARGET     quicksort.random.plain)
set(PLAIN_TARGET_S   quicksort.random.plain.s)
set(ENCODED_TARGET   quicksort.random.encoded)
set(ENCODED_TARGET_S quicksort.random.encoded.s)


# Build the bitcode file containing the input array:
ADD_BC_COMMAND(${INPUT_BC} ${INPUT_C} "${FLAG_OPTS};${FLAG_DEBUG}")


# Build the bitcode file for the main application:
set(FLAGS ${FLAG_DEBUG} ${FLAG_OPTS}
          -I${UTILS_INC_PATH}
          -DLENGTH=${LENGTH}
          -DREPETITIONS=${REPETITIONS}
          ${PREP_DEBUG}
          ${PREP_CYCLES}
          ${PREP_CHECKSUM})
ADD_BC_COMMAND(${QUICKSORT_MAIN_BC} ${QUICKSORT_MAIN_C} "${FLAGS}")


# Build the bitcode file to be encoded:
set(FLAGS ${FLAG_DEBUG} -O0 -mno-sse)
ADD_BC_COMMAND(${QUICKSORT_ENC_BC} ${QUICKSORT_ENC_C} "${FLAGS}")


# Encode:
ADD_ENCODED_BC_COMMAND(${ENCODED_BC} ${QUICKSORT_ENC_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_LL} ${ENCODED_BC} "")

ADD_ENCODED_BC_COMMAND(${ENCODED_INPUT_BC} ${INPUT_BC} "")
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

