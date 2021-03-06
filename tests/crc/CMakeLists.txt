cmake_minimum_required(VERSION 2.8)

include(../../cmake/functions.cmake)
include(../../cmake/flags.cmake)


set(CRC_MAIN_C    ${CMAKE_CURRENT_SOURCE_DIR}/crc.main.c)
set(CRC_MAIN_BC   crc.main.bc)
set(CRC_ENC_C     ${CMAKE_CURRENT_SOURCE_DIR}/crc.enc.c)
set(CRC_ENC_BC    crc.enc.bc)
set(INPUT_C       ${CMAKE_CURRENT_SOURCE_DIR}/crc.input.c)
set(INPUT_BC      crc.input.bc)
set(CRC_32_TAB_C  ${CMAKE_CURRENT_SOURCE_DIR}/crc.crc_32_tab.c)
set(CRC_32_TAB_BC crc.crc_32_tab.bc)


set(ENCODED_BC            crc.enc.encoded.bc)
set(ENCODED_LL            crc.enc.encoded.ll)
set(ENCODED_INPUT_BC      crc.input.encoded.bc)
set(ENCODED_INPUT_LL      crc.input.encoded.ll)
set(ENCODED_CRC_32_TAB_BC crc.crc_32_tab.encoded.bc)
set(ENCODED_CRC_32_TAB_LL crc.crc_32_tab.encoded.ll)


set(PLAIN_TARGET     crc.plain)
set(PLAIN_TARGET_S   crc.plain.s)
set(ENCODED_TARGET   crc.encoded)
set(ENCODED_TARGET_S crc.encoded.s)


# Build bitcode files containing arrays of data:
ADD_BC_TARGET(${INPUT_BC} ${INPUT_C} "${FLAG_OPTS};${FLAG_DEBUG}")
ADD_BC_TARGET(${CRC_32_TAB_BC} ${CRC_32_TAB_C} "${FLAG_OPTS};${FLAG_DEBUG}")


# Build the bitcode file for the main application:
set(FLAGS ${FLAG_DEBUG} ${FLAG_OPTS}
          -I${UTILS_INC_PATH}
          -DLENGTH=${LENGTH}
          -DREPETITIONS=${REPETITIONS}
          ${PREP_DEBUG}
          ${PREP_CYCLES}
          ${PREP_CHECKSUM})
ADD_BC_TARGET(${CRC_MAIN_BC} ${CRC_MAIN_C} "${FLAGS}")


# Build the bitcode file to be encoded:
set(FLAGS ${FLAG_DEBUG} -O0 -mno-sse)
ADD_BC_TARGET(${CRC_ENC_BC} ${CRC_ENC_C} "${FLAGS}")


# Encode:
ADD_ENCODED_BC_TARGET(${ENCODED_BC} ${CRC_ENC_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_LL} ${ENCODED_BC} "")

ADD_ENCODED_BC_TARGET(${ENCODED_INPUT_BC} ${INPUT_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_INPUT_LL} ${ENCODED_INPUT_BC} "")

ADD_ENCODED_BC_TARGET(${ENCODED_CRC_32_TAB_BC} ${CRC_32_TAB_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_CRC_32_TAB_LL} ${ENCODED_CRC_32_TAB_BC} "")


# Link the 'plain' application binary:
ADD_BIN_TARGET(${PLAIN_TARGET}
               "${CRC_MAIN_BC};${CRC_ENC_BC};${INPUT_BC};${CRC_32_TAB_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${PLAIN_TARGET_S} ${PLAIN_TARGET})


# Link the 'encoded' application binary:
ADD_BIN_TARGET(${ENCODED_TARGET}
               "${CRC_MAIN_BC};${ENCODED_BC};${ENCODED_INPUT_BC};${ENCODED_CRC_32_TAB_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${ENCODED_TARGET_S} ${ENCODED_TARGET})


# Generate reference output from the plain binary:
ADD_OUTPUT_TARGET("crc.out"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PLAIN_TARGET};--cso;crc.out"
                  ${PLAIN_TARGET})


if(BUILD_CONFIGS)
  add_subdirectory(configs)
endif(BUILD_CONFIGS)
