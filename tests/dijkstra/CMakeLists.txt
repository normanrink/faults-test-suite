cmake_minimum_required(VERSION 2.8)

include(../../cmake/functions.cmake)
include(../../cmake/flags.cmake)


set(DIJKSTRA_MAIN_C  ${CMAKE_CURRENT_SOURCE_DIR}/dijkstra.main.c)
set(DIJKSTRA_MAIN_BC dijkstra.main.bc)
set(DIJKSTRA_ENC_C   ${CMAKE_CURRENT_SOURCE_DIR}/dijkstra.enc.c)
set(DIJKSTRA_ENC_BC  dijkstra.enc.bc)
set(ADJACENCY_C      ${CMAKE_CURRENT_SOURCE_DIR}/dijkstra.adjacency.c)
set(ADJACENCY_BC     dijkstra.adjacency.bc)


set(ENCODED_BC           dijkstra.enc.encoded.bc)
set(ENCODED_LL           dijkstra.enc.encoded.ll)
set(ENCODED_ADJACENCY_BC dijkstra.adjacency.encoded.bc)
set(ENCODED_ADJACENCY_LL dijkstra.adjacency.encoded.ll)


set(PLAIN_TARGET     dijkstra.plain)
set(PLAIN_TARGET_S   dijkstra.plain.s)
set(ENCODED_TARGET   dijkstra.encoded)
set(ENCODED_TARGET_S dijkstra.encoded.s)


# Build the bitcode file containing the adjacency matrix:
ADD_BC_COMMAND(${ADJACENCY_BC} ${ADJACENCY_C} "${FLAG_OPTS};${FLAG_DEBUG}")


# Build the bitcode file for the main application:
set(FLAGS ${FLAG_DEBUG} ${FLAG_OPTS}
          -I${UTILS_INC_PATH}
          -DLENGTH=${LENGTH}
          -DREPETITIONS=${REPETITIONS}
          ${PREP_DEBUG}
          ${PREP_CYCLES}
          ${PREP_CHECKSUM})
ADD_BC_COMMAND(${DIJKSTRA_MAIN_BC} ${DIJKSTRA_MAIN_C} "${FLAGS}")


# Build the bitcode file to be encoded:
set(FLAGS ${FLAG_DEBUG} -O0 -mno-sse)
ADD_BC_COMMAND(${DIJKSTRA_ENC_BC} ${DIJKSTRA_ENC_C} "${FLAGS}")


# Encode:
ADD_ENCODED_BC_COMMAND(${ENCODED_BC} ${DIJKSTRA_ENC_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_LL} ${ENCODED_BC} "")

ADD_ENCODED_BC_COMMAND(${ENCODED_ADJACENCY_BC} ${ADJACENCY_BC} "")
ADD_LL_DIS_TARGET(${ENCODED_ADJACENCY_LL} ${ENCODED_ADJACENCY_BC} "")


# Link the 'plain' application binary:
ADD_BIN_TARGET(${PLAIN_TARGET}
               "${DIJKSTRA_MAIN_BC};${DIJKSTRA_ENC_BC};${ADJACENCY_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${PLAIN_TARGET_S} ${PLAIN_TARGET})


# Link the 'encoded' application binary:
ADD_BIN_TARGET(${ENCODED_TARGET}
               "${DIJKSTRA_MAIN_BC};${ENCODED_BC};${ENCODED_ADJACENCY_BC}"
               "${FLAG_DEBUG};${FLAG_OPTS}"
               "-L${UTILS_LIB_PATH};-l${UTILS_LIB}"
               ${UTILS_LIB})
ADD_DISASM_TARGET(${ENCODED_TARGET_S} ${ENCODED_TARGET})

