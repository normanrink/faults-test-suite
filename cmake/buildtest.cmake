cmake_minimum_required(VERSION 2.8)


if(NOT DEFINED ENCODER OR NOT ENCODER)
  message(FATAL_ERROR "'ENCODER' not set")
endif(NOT DEFINED ENCODER OR NOT ENCODER)

if(NOT DEFINED ENCODING_PROFILE OR NOT ENCODING_PROFILE)
  message(FATAL_ERROR "'ENCODING_PROFILE' not set")
endif(NOT DEFINED ENCODING_PROFILE OR NOT ENCODING_PROFILE)


# Set build options/compiler flags:
set(ENCODE_OPTS -p ${ENCODING_PROFILE})
# More compiler flags:
if("${CMAKE_BUILD_TYPE}" MATCHES "DEBUG" OR "${CMAKE_BUILD_TYPE}" MATCHES "Debug")
  set(CLANG_EMIT_LLVM_OPTS -O0)
  set(CLANG_BACKEND_OPTS -O0)
  set(ENCODE_OPTS ${ENCODE_OPTS} -no-inlining -no-opts)
  set(DEBUG_OPTS -g)
else()
  set(CLANG_EMIT_LLVM_OPTS -O2)
  set(CLANG_BACKEND_OPTS -O2)
endif("${CMAKE_BUILD_TYPE}" MATCHES "DEBUG" OR "${CMAKE_BUILD_TYPE}" MATCHES "Debug")


if(NOT DEFINED LENGTH OR NOT LENGTH)
  message(FATAL_ERROR "'LENGTH' not set")
endif(NOT DEFINED LENGTH OR NOT LENGTH)

if(NOT DEFINED REPETITIONS OR NOT REPETITIONS)
  message(FATAL_ERROR "'REPETITIONS' not set")
endif(NOT DEFINED REPETITIONS OR NOT REPETITIONS)

# Define macros for the C-preprocessor:
set(PP_DEFS -DLENGTH=${LENGTH})
set(PP_DEFS ${PP_DEFS} -DREPETITIONS=${REPETITIONS})


if(NOT DEFINED TEST_NAME OR NOT TEST_NAME)
  message(FATAL_ERROR "'TEST_NAME' not set")
endif(NOT DEFINED TEST_NAME OR NOT TEST_NAME)

if(NOT DEFINED TARGET_INFIX OR NOT TARGET_INFIX)
  message(FATAL_ERROR "'TARGET_INFIX' not set")
endif(NOT DEFINED TARGET_INFIX OR NOT TARGET_INFIX)

if(NOT DEFINED MAIN_SRC OR NOT MAIN_SRC)
  message(FATAL_ERROR "'MAIN_SRC' not set")
endif(NOT DEFINED MAIN_SRC OR NOT MAIN_SRC)

if(NOT DEFINED ENC_SRC OR NOT ENC_SRC)
  message(FATAL_ERROR "'ENC_SRC' not set")
endif(NOT DEFINED ENC_SRC OR NOT ENC_SRC)


set(TARGET ${TEST_NAME}.${TEST_INFIX})
set(REFERENCE reference.${TEST_INFIX}.out)




  set(MAIN_MODULE_ENC_BC  "${TARGET_NAME}.main.enc.bc")
  set(MAIN_MODULE_ENC_2_BC  "${TARGET_NAME}.main.enc.2.bc")
  set(ENC_MODULE_ENC_BC   "${TARGET_NAME}.enc.enc.bc")
  set(ENC_MODULE_MACRO_BC   "${TARGET_NAME}.enc.macro.bc")

  set(PLAIN_BC "${TARGET_NAME}.plain.bc")
  set(PLAIN_TARGET "${TARGET_NAME}.plain")
  set(ENC_TARGET  "${TARGET_NAME}.encoded")
  set(ENC_TARGET_S  "${TARGET_NAME}.encoded.s")
  set(ENC_TARGET_BC  "${TARGET_NAME}.encoded.bc")
endmacro(SET_BUILD_VARS)


# Build 'plain' test:
set(MAIN_BC      "${TARGET}.main.plain.bc")
set(ENC_BC       "${TARGET}.enc.plain.bc")
set(PLAIN_BC     "${TARGET}.plain.bc")
set(PLAIN_TARGET "${TARGET}.plain")

add_custom_command(OUTPUT ${MAIN_BC}
                   # pass option "-O2" to ensure that functions from header
                   # files: are inlined:
                   COMMAND ${CLANG} -c -emit-llvm ${DEBUG_OPTS} -O2
                           ${PP_DEFS}
                           -o ${MAIN_BC}
                           ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_SRC}
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_SRC})

add_custom_command(OUTPUT ${ENC_BC}
                   COMMAND ${CLANG} -c -emit-llvm
                           ${DEBUG_OPTS} ${CLANG_EMIT_LLVM_OPTS} -mno-sse
                           ${PP_DEFS}
                           -o ${ENC_BC}
                           ${CMAKE_CURRENT_SOURCE_DIR}/${ENC_SRC}
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${ENC_SRC})

add_custom_command(OUTPUT ${PLAIN_BC}
                  COMMAND ${LLVM_LINK}
                          -o ${PLAIN_BC}
                          ${MAIN_BC} ${ENC_BC} mycheck.bc mycyc.bc
                  DEPENDS ${MAIN_BC} ${ENC_BC} mycheck.bc mycyc.bc)

add_custom_command(OUTPUT ${PLAIN_TARGET}
                  COMMAND ${CLANG} ${CLANG_BACKEND_OPTS}
                          -o ${PLAIN_TARGET}
                          ${PLAIN_BC}
                  DEPENDS ${PLAIN_BC})

add_custom_command(OUTPUT ${REFERENCE}
                   COMMAND ${PLAIN_TARGET} --cso ${REFERENCE}
                   DEPENDS ${PLAIN_TARGET}

  # Build encoded binary:
  add_custom_command(OUTPUT ${MAIN_MODULE_ENC_BC}
		     # pass option "-O2" to ensure that functions from headers files are inlined:
                     COMMAND ${CLANG} -c -emit-llvm ${DEBUG_OPTS} -O2
			     -DENCODE
                             ${PP_DEFS} -o ${MAIN_MODULE_ENC_BC}
                             ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_MODULE_SRC}
                     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_MODULE_SRC})

  add_custom_command(OUTPUT ${MAIN_MODULE_ENC_2_BC}
                     COMMAND ${ENCODE_BIN_DIR}/encode -expand-only
                             ${ENCODE_OPTS}
                             -o ${MAIN_MODULE_ENC_2_BC} ${MAIN_MODULE_ENC_BC}
                     DEPENDS encode ${MAIN_MODULE_ENC_BC})

  add_custom_command(OUTPUT ${ENC_MODULE_MACRO_BC}
                     COMMAND ${CLANG} -c -emit-llvm ${DEBUG_OPTS}
			     ${CLANG_EMIT_LLVM_OPTS} -mno-sse
                             -DENCODE
                             -o ${ENC_MODULE_MACRO_BC}
                             ${CMAKE_CURRENT_SOURCE_DIR}/${ENC_MODULE_SRC}
                     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${ENC_MODULE_SRC})

  add_custom_command(OUTPUT ${ENC_MODULE_ENC_BC}
                     COMMAND ${ENCODE_BIN_DIR}/encode ${ENCODE_OPTS}
                             -o ${ENC_MODULE_ENC_BC}
                             ${ENC_MODULE_MACRO_BC}
                     DEPENDS encode ${ENC_MODULE_MACRO_BC})

  add_custom_target(${ENC_TARGET_BC} ALL
                    COMMAND ${LINK} -o ${ENC_TARGET_BC}
                            ${MAIN_MODULE_ENC_2_BC} ${ENC_MODULE_ENC_BC}
                            ../mylibs/mycheck.bc
                            ../mylibs/mycyc.bc
                    DEPENDS ${MAIN_MODULE_ENC_2_BC} ${ENC_MODULE_ENC_BC}
                            mycheck.bc
                            mycyc.bc)
  
  add_custom_target(${ENC_TARGET} ALL
                    COMMAND ${CLANG} ${DEBUG_OPTS} ${CLANG_LINK_OPTS}
                            -o ${ENC_TARGET}
                            ${ENC_TARGET_BC}
                    DEPENDS ${ENC_TARGET_BC})
  add_custom_target(${ENC_TARGET_S} ALL
                    COMMAND ${CLANG} ${DEBUG_OPTS} ${CLANG_LINK_OPTS}
                            -S -o ${ENC_TARGET_S}
                            ${ENC_TARGET_BC}
                    DEPENDS ${ENC_TARGET_BC})
endfunction(BUILD_TEST_CASE)

