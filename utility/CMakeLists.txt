cmake_minimum_required(VERSION 2.8)


if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  set(OPTS -g -O0)
else()
  set(OPTS -O2)
endif(CMAKE_BUILD_TYPE MATCHES DEBUG)


if(MAJORITY_VOTING)
  set(VOTING -DMAJORITY_VOTING)
endif(MAJORITY_VOTING)

if(VERBOSE_VOTING)
  set(VOTING ${VOTING} -DVERBOSE_VOTING)
endif(VERBOSE_VOTING)

set(UTILS_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")
set(UTILS_LIB_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")
set(UTILS_LIB "utils" CACHE INTERNAL "")

add_library(${UTILS_LIB} mycheck.c
                         mycyc.c
                         myargs.c)

if(VOTING)
  set_target_properties(${UTILS_LIB}
                        PROPERTIES COMPILE_FLAGS ${VOTING})
endif(VOTING)

