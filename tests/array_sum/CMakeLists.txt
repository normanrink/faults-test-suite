cmake_minimum_required(VERSION 2.8)

include(../../cmake/functions.cmake)
include(../../cmake/flags.cmake)


set(ARRAY_SUM_MAIN_C  ${CMAKE_CURRENT_SOURCE_DIR}/array_sum.main.c)
set(ARRAY_SUM_MAIN_BC array_sum.main.bc)
set(ARRAY_SUM_ENC_C   ${CMAKE_CURRENT_SOURCE_DIR}/array_sum.enc.c)
set(ARRAY_SUM_ENC_BC  array_sum.enc.bc)
set(INPUT_C           ${CMAKE_CURRENT_SOURCE_DIR}/array_sum.input.c)
set(INPUT_BC          array_sum.input.bc)


set(ENCODED_BC       array_sum.enc.encoded.bc)
set(ENCODED_LL       array_sum.enc.encoded.ll)
set(ENCODED_INPUT_BC array_sum.input.encoded.bc)
set(ENCODED_INPUT_LL array_sum.input.encoded.ll)


set(PLAIN_TARGET     array_sum.plain)
set(PLAIN_TARGET_S   array_sum.plain.s)
set(ENCODED_TARGET   array_sum.encoded)
set(ENCODED_TARGET_S array_sum.encoded.s)


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
ADD_BC_TARGET(${ARRAY_SUM_MAIN_BC} ${ARRAY_SUM_MAIN_C} "${FLAGS}")


# Build the bitcode file to be encoded:
set(FLAGS ${FLAG_DEBUG} -O0 -mno-sse)
ADD_BC_TARGET(${ARRAY_SUM_ENC_BC} ${ARRAY_SUM_ENC_C} "${FLAGS}")


# Encode:
ADD_ENCODED_BC_TARGET(${ENCODED_BC} ${ARRAY_SUM_ENC_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_LL} ${ENCODED_BC} "")

ADD_ENCODED_BC_TARGET(${ENCODED_INPUT_BC} ${INPUT_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_INPUT_LL} ${ENCODED_INPUT_BC} "")


# Link the 'plain' application binary:
ADD_BIN_TARGET(${PLAIN_TARGET}
               "${ARRAY_SUM_MAIN_BC};${ARRAY_SUM_ENC_BC};${INPUT_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${PLAIN_TARGET_S} ${PLAIN_TARGET})


# Link the 'encoded' application binary:
ADD_BIN_TARGET(${ENCODED_TARGET}
               "${ARRAY_SUM_MAIN_BC};${ENCODED_BC};${ENCODED_INPUT_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${ENCODED_TARGET_S} ${ENCODED_TARGET})

