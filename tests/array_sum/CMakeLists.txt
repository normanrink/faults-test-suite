cmake_minimum_required(VERSION 2.8)

include(../../cmake/build-test-2.cmake)


set(ARRAY_SUM_MAIN_C  ${CMAKE_CURRENT_SOURCE_DIR}/array_sum.main.c)
set(ARRAY_SUM_MAIN_BC array_sum.main.bc)
set(ARRAY_SUM_ENC_C   ${CMAKE_CURRENT_SOURCE_DIR}/array_sum.enc.c)
set(ARRAY_SUM_ENC_BC  array_sum.enc.bc)


set(ENCODED_BC array_sum.enc.encoded.bc)
set(ENCODED_LL array_sum.enc.encoded.ll)


set(PLAIN_TARGET     array_sum.plain)
set(PLAIN_TARGET_S   array_sum.plain.s)
set(ENCODED_TARGET   array_sum.encoded)
set(ENCODED_TARGET_S array_sum.encoded.s)


if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  set(OPTS -g -O0)
  set(DBG -g)
else()
  set(OPTS -O3)
endif(CMAKE_BUILD_TYPE MATCHES DEBUG)


# Build the bitcode file for the main application:
set(FLAGS ${OPTS} -I${UTILS_INC_PATH} -DLENGTH=${LENGTH}
                                      -DREPETITIONS=${REPETITIONS})
ADD_BC_COMMAND(${ARRAY_SUM_MAIN_BC} ${ARRAY_SUM_MAIN_C} "${FLAGS}")


# Build the bitcode file to be encoded:
set(FLAGS -I${UTILS_INC_PATH} ${DBG} -O0 -mno-sse)
ADD_BC_COMMAND(${ARRAY_SUM_ENC_BC} ${ARRAY_SUM_ENC_C} "${FLAGS}")


# Encode:
ADD_ENCODED_BC_COMMAND(${ENCODED_BC} ${ARRAY_SUM_ENC_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_LL} ${ENCODED_BC} "")


# Link the 'plain' application binary:
ADD_BIN_TARGET(${PLAIN_TARGET}
               "${ARRAY_SUM_MAIN_BC};${ARRAY_SUM_ENC_BC}"
               "${OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${PLAIN_TARGET_S} ${PLAIN_TARGET})


# Link the 'encoded' application binary:
ADD_BIN_TARGET(${ENCODED_TARGET}
               "${ARRAY_SUM_MAIN_BC};${ENCODED_BC}"
               "${OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${ENCODED_TARGET_S} ${ENCODED_TARGET})

