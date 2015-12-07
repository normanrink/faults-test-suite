cmake_minimum_required(VERSION 2.8)

include(../../cmake/functions.cmake)
include(../../cmake/flags.cmake)


set(PTRARITH_MAIN_C  ${CMAKE_CURRENT_SOURCE_DIR}/ptrarith.main.c)
set(PTRARITH_MAIN_BC ptrarith.main.bc)
set(PTRARITH_ENC_C   ${CMAKE_CURRENT_SOURCE_DIR}/ptrarith.enc.c)
set(PTRARITH_ENC_BC  ptrarith.enc.bc)
set(SOURCE_C         ${CMAKE_CURRENT_SOURCE_DIR}/ptrarith.source.c)
set(SOURCE_BC        ptrarith.source.bc)


set(ENCODED_BC        ptrarith.enc.encoded.bc)
set(ENCODED_LL        ptrarith.enc.encoded.ll)
set(ENCODED_SOURCE_BC ptrarith.source.encoded.bc)
set(ENCODED_SOURCE_LL ptrarith.source.encoded.ll)


set(PLAIN_TARGET     ptrarith.plain)
set(PLAIN_TARGET_S   ptrarith.plain.s)
set(ENCODED_TARGET   ptrarith.encoded)
set(ENCODED_TARGET_S ptrarith.encoded.s)


# Build the bitcode file containing the source array:
ADD_BC_TARGET(${SOURCE_BC} ${SOURCE_C} "${FLAG_OPTS};${FLAG_DEBUG}")


# Build the bitcode file for the main application:
set(FLAGS ${FLAG_DEBUG} ${FLAG_OPTS}
          -I${UTILS_INC_PATH}
          -DLENGTH=${LENGTH}
          -DREPETITIONS=${REPETITIONS}
          ${PREP_DEBUG}
          ${PREP_CYCLES}
          ${PREP_CHECKSUM})
ADD_BC_TARGET(${PTRARITH_MAIN_BC} ${PTRARITH_MAIN_C} "${FLAGS}")


# Build the bitcode file to be encoded:
set(FLAGS ${FLAG_DEBUG} -O0 -mno-sse)
ADD_BC_TARGET(${PTRARITH_ENC_BC} ${PTRARITH_ENC_C} "${FLAGS}")


# Encode:
ADD_ENCODED_BC_TARGET(${ENCODED_BC} ${PTRARITH_ENC_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_LL} ${ENCODED_BC} "")

ADD_ENCODED_BC_TARGET(${ENCODED_SOURCE_BC} ${SOURCE_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_SOURCE_LL} ${ENCODED_SOURCE_BC} "")


# Link the 'plain' application binary:
ADD_BIN_TARGET(${PLAIN_TARGET}
               "${PTRARITH_MAIN_BC};${PTRARITH_ENC_BC};${SOURCE_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${PLAIN_TARGET_S} ${PLAIN_TARGET})


# Link the 'encoded' application binary:
ADD_BIN_TARGET(${ENCODED_TARGET}
               "${PTRARITH_MAIN_BC};${ENCODED_BC};${ENCODED_SOURCE_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${ENCODED_TARGET_S} ${ENCODED_TARGET})


# Generate reference output from the plain binary:
ADD_OUTPUT_TARGET("ptrarith.out"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PLAIN_TARGET};--cso;ptrarith.out"
                  ${PLAIN_TARGET})


if(BUILD_CONFIGS)
  add_subdirectory(configs)
endif(BUILD_CONFIGS)
