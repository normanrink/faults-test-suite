cmake_minimum_required(VERSION 2.8)


if("${CMAKE_BUILD_TYPE}" MATCHES "DEBUG" OR "${CMAKE_BUILD_TYPE}" MATCHES "Debug")
  set(CLANG_EMIT_LLVM_OPTS -O0)
  set(DEBUG_OPTS -g)
else()
  set(CLANG_EMIT_LLVM_OPTS -O2)
endif("${CMAKE_BUILD_TYPE}" MATCHES "DEBUG" OR "${CMAKE_BUILD_TYPE}" MATCHES "Debug")


if(MAJORITY_VOTING)
  set(VOTING -DMAJORITY_VOTING)
endif(MAJORITY_VOTING)

if(VERBOSE_VOTING)
  set(VOTING ${VOTING} -DVERBOSE_VOTING)
endif(VERBOSE_VOTING)

# Compile bitcode libraries:
add_custom_target(mycheck.bc ALL
                  COMMAND ${CLANG} -c -emit-llvm
                          ${DEBUG_OPTS} ${CLANG_EMIT_LLVM_OPTS}
                          ${VOTING}
                          -o mycheck.bc
                          ${CMAKE_CURRENT_SOURCE_DIR}/mycheck.c
                  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mycheck.c)

add_custom_target(mycyc.bc ALL
                  COMMAND ${CLANG} -c -emit-llvm
                          ${DEBUG_OPTS} ${CLANG_EMIT_LLVM_OPTS}
                          ${VOTING}
                          -o mycyc.bc
                          ${CMAKE_CURRENT_SOURCE_DIR}/mycyc.c
                  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mycyc.c)

add_custom_target(myargs.bc ALL
                  COMMAND ${CLANG} -c -emit-llvm
                          ${DEBUG_OPTS} ${CLANG_EMIT_LLVM_OPTS}
                          ${VOTING}
                          -o myargs.bc
                          ${CMAKE_CURRENT_SOURCE_DIR}/myargs.c
                  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/myargs.c)

