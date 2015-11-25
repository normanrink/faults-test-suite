cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test-2.cmake)


set(CRC_MAIN_C    ${CMAKE_CURRENT_SOURCE_DIR}/crc.main.c)
set(CRC_MAIN_BC   crc.main.bc)
set(CRC_ENC_C     ${CMAKE_CURRENT_SOURCE_DIR}/crc.enc.c)
set(CRC_ENC_BC    crc.enc.bc)
set(INPUT_C       ${CMAKE_CURRENT_SOURCE_DIR}/input.c)
set(INPUT_BC      input.bc)
set(CRC_32_TAB_C  ${CMAKE_CURRENT_SOURCE_DIR}/crc_32_tab.c)
set(CRC_32_TAB_BC crc_32_tab.bc)


set(ENCODED_BC            crc.enc.encoded.bc)
set(ENCODED_LL            crc.enc.encoded.ll)
set(ENCODED_INPUT_BC      input.encoded.bc)
set(ENCODED_INPUT_LL      input.encoded.ll)
set(ENCODED_CRC_32_TAB_BC crc_32_tab.encoded.bc)
set(ENCODED_CRC_32_TAB_LL crc_32_tab.encoded.ll)


set(PLAIN_TARGET     crc.plain)
set(PLAIN_TARGET_S   crc.plain.s)
set(ENCODED_TARGET   crc.encoded)
set(ENCODED_TARGET_S crc.encoded.s)


if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  set(OPTS -O0 -g)
  set(DBG -g)
else()
  set(OPTS -O3)
endif(CMAKE_BUILD_TYPE MATCHES DEBUG)


# Build bitcode files containing arrays of data:
ADD_BC_COMMAND(${INPUT_BC} ${INPUT_C} "${OPTS}")
ADD_BC_COMMAND(${CRC_32_TAB_BC} ${CRC_32_TAB_C} "${OPTS}")


# Build the bitcode file for the main application:
set(FLAGS ${OPTS} -I${UTILS_INC_PATH} -DLENGTH=${LENGTH}
                                      -DREPETITIONS=${REPETITIONS})
ADD_BC_COMMAND(${CRC_MAIN_BC} ${CRC_MAIN_C} "${FLAGS}")


# Build the bitcode file to be encoded:
set(FLAGS -I${UTILS_INC_PATH} ${DBG} -O0 -mno-sse)
ADD_BC_COMMAND(${CRC_ENC_BC} ${CRC_ENC_C} "${FLAGS}")


# Encode:
ADD_ENCODED_BC_COMMAND(${ENCODED_BC} ${CRC_ENC_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_LL} ${ENCODED_BC} "")

ADD_ENCODED_BC_COMMAND(${ENCODED_INPUT_BC} ${INPUT_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_INPUT_LL} ${ENCODED_INPUT_BC} "")

ADD_ENCODED_BC_COMMAND(${ENCODED_CRC_32_TAB_BC} ${CRC_32_TAB_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_CRC_32_TAB_LL} ${ENCODED_CRC_32_TAB_BC} "")


# Link the 'plain' application binary:
ADD_BIN_TARGET(${PLAIN_TARGET}
               "${CRC_MAIN_BC};${CRC_ENC_BC};${INPUT_BC};${CRC_32_TAB_BC}"
               "${OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${PLAIN_TARGET_S} ${PLAIN_TARGET})


# Link the 'encoded' application binary:
ADD_BIN_TARGET(${ENCODED_TARGET}
               "${CRC_MAIN_BC};${ENCODED_BC};${ENCODED_INPUT_BC};${ENCODED_CRC_32_TAB_BC}"
               "${OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${ENCODED_TARGET_S} ${ENCODED_TARGET})


# Generate reference output from the plain binary:
ADD_OUTPUT_TARGET("crc.out"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PLAIN_TARGET};--cso;crc.out"
                  ${PLAIN_TARGET})

