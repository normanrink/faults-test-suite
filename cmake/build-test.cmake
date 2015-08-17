cmake_minimum_required(VERSION 2.8)


if(NOT DEFINED ENCODER OR NOT ENCODER)
  message(FATAL_ERROR "'ENCODER' not set")
endif(NOT DEFINED ENCODER OR NOT ENCODER)

if(NOT DEFINED ENCODING_PROFILE OR NOT ENCODING_PROFILE)
  message(FATAL_ERROR "'ENCODING_PROFILE' not set")
endif(NOT DEFINED ENCODING_PROFILE OR NOT ENCODING_PROFILE)


# Set build options/compiler flags:
if("${CMAKE_BUILD_TYPE}" MATCHES "DEBUG" OR "${CMAKE_BUILD_TYPE}" MATCHES "Debug")
  set(CLANG_EMIT_LLVM_OPTS -O0)
  set(CLANG_BACKEND_OPTS -O0)
  set(ENCODE_OPTS -no-inlining -no-opts)
  set(DEBUG_OPTS -g)
else()
  set(CLANG_EMIT_LLVM_OPTS -O2)
  set(CLANG_BACKEND_OPTS -O2)
endif("${CMAKE_BUILD_TYPE}" MATCHES "DEBUG" OR "${CMAKE_BUILD_TYPE}" MATCHES "Debug")


function(BUILD_PLAIN_TEST
         TEST_NAME TEST_INFIX MAIN_SRC ENC_SRC COVER_TEST LENGTH REPETITIONS)
# Define macros for the C-preprocessor:
  set(PP_DEFS -DLENGTH=${LENGTH})
  set(PP_DEFS ${PP_DEFS} -DREPETITIONS=${REPETITIONS})
# Set a project-specific include path:
  set(INC_PATH -I${INC_LIBS})

# Set variabled for 'plain' test:
  set(TARGET       ${TEST_NAME}.${TEST_INFIX})
  set(MAIN_BC      "${TARGET}.main.plain.bc")
  set(ENC_BC       "${TARGET}.enc.plain.bc")
  set(PLAIN_BC     "${TARGET}.plain.bc")
  set(PLAIN_TARGET "${TARGET}.plain")

# Build 'plain' test:
# Generate bitcode for the 'main' module:
  add_custom_command(OUTPUT ${MAIN_BC}
                     # pass option "-O2" to ensure that functions from header
                     # files are inlined:
                     COMMAND ${CLANG} -c -emit-llvm ${DEBUG_OPTS} -O2
                             ${PP_DEFS}
                             ${INC_PATH}
                             -o ${MAIN_BC}
                             ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_SRC}
                     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_SRC})
# Generate bitcode for the 'enc' module:
  add_custom_command(OUTPUT ${ENC_BC}
                     COMMAND ${CLANG} -c -emit-llvm
                             ${DEBUG_OPTS} ${CLANG_EMIT_LLVM_OPTS} -mno-sse
                             ${PP_DEFS}
                             ${INC_PATH}
                             -o ${ENC_BC}
                             ${CMAKE_CURRENT_SOURCE_DIR}/${ENC_SRC}
                     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${ENC_SRC})
  if(${COVER_TEST})
    set(ENC_COVER_BC "${TARGET}.enc.plain.cover.bc")
    add_custom_command(OUTPUT ${ENC_COVER_BC}
                       COMMAND ${OPT} -load ${NO_ENC_INLINE_LIB}
                               -NoEncInline ${ENC_BC}
                               -o ${ENC_COVER_BC}
                       DEPENDS ${ENC_BC})
    set(ENC_BC ${ENC_COVER_BC})
  endif(${COVER_TEST})
# Link both modules:
  add_custom_command(OUTPUT ${PLAIN_BC}
                    COMMAND ${LLVM_LINK}
                            -o ${PLAIN_BC}
                            ${MAIN_BC} ${ENC_BC}
                            ${BC_LIBS}/myargs.bc
                            ${BC_LIBS}/mycheck.bc
                            ${BC_LIBS}/mycyc.bc
                    DEPENDS ${MAIN_BC} ${ENC_BC} mycheck.bc mycyc.bc myargs.bc)
# Build the 'plain' executable:
  add_custom_target(${PLAIN_TARGET} ALL
                    COMMAND ${CLANG} ${CLANG_BACKEND_OPTS}
                            -o ${PLAIN_TARGET}
                            ${PLAIN_BC}
                    DEPENDS ${PLAIN_BC})
endfunction(BUILD_PLAIN_TEST)


function(BUILD_REF_OUTPUT TEST_NAME TEST_INFIX ARGS)
# Set common variables:
  set(TARGET       ${TEST_NAME}.${TEST_INFIX})
  set(PLAIN_TARGET "${TARGET}.plain")
  set(REFERENCE    reference.${TEST_INFIX}.out)
  set(REF_TARGET   reference.${TARGET}.out)

# Generate reference output:
  string(REPLACE ";" " " $ARGS $ARGS)
  add_custom_target(${REF_TARGET} ALL
                    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PLAIN_TARGET}
                            ${ARGS}
                            --cso ${REFERENCE}
                    DEPENDS ${PLAIN_TARGET})
endfunction(BUILD_REF_OUTPUT)


function(BUILD_ENCODED_TEST
         TEST_NAME TEST_INFIX MAIN_SRC ENC_SRC COVER_TEST LENGTH REPETITIONS)
# Define macros for the C-preprocessor:
  set(PP_DEFS -DLENGTH=${LENGTH})
  set(PP_DEFS ${PP_DEFS} -DREPETITIONS=${REPETITIONS})
# Set a project-specific include path:
  set(INC_PATH -I${INC_LIBS})

# Set common variables:
  set(TARGET ${TEST_NAME}.${TEST_INFIX})

# Set variabled for 'encoded' test:
  set(MAIN_BC        "${TARGET}.main.encoded.bc")
  set(MAIN_TMP_BC    "${TARGET}.main.encoded.tmp.bc")
  set(ENC_BC         "${TARGET}.enc.encoded.bc")
  set(ENC_TMP_BC     "${TARGET}.enc.encoded.tmp.bc")
  set(ENCODED_BC     "${TARGET}.encoded.bc")
  set(ENCODED_S      "${TARGET}.encoded.s")
  set(ENCODED_TARGET "${TARGET}.encoded")

# Build 'encoded' test:
# Generate bitcode for the 'main' module:
# (Since macros are expanded to encoding builtins, we must run the ${ENCODER}.)
  add_custom_command(OUTPUT ${MAIN_BC}
		     # pass option "-O2" to ensure that functions from header
                     # files are inlined:
                     COMMAND ${CLANG} -c -emit-llvm ${DEBUG_OPTS} -O2
			     -DENCODE ${PP_DEFS}
                             ${INC_PATH}
                             -o ${MAIN_TMP_BC}
                             ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_SRC}
                     COMMAND ${ENCODER} -expand-only
                             ${ENCODE_OPTS}
                             -o ${MAIN_BC}
                             ${MAIN_TMP_BC}
                     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_SRC})
# Generate bitcode for the 'enc' module:
  add_custom_command(OUTPUT ${ENC_BC}
                     COMMAND ${CLANG} -c -emit-llvm
                             ${DEBUG_OPTS} ${CLANG_EMIT_LLVM_OPTS} -mno-sse
                             -DENCODE ${PP_DEFS}
                             ${INC_PATH}
                             -o ${ENC_TMP_BC}
                             ${CMAKE_CURRENT_SOURCE_DIR}/${ENC_SRC}
                     COMMAND ${ENCODER}
                             ${ENCODE_OPTS} -p ${ENCODING_PROFILE} -disable-enc-inlining
                             -o ${ENC_BC}
                             ${ENC_TMP_BC}
                     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${ENC_SRC})
  if(${COVER_TEST})
    set(ENC_COVER_BC "${TARGET}.enc.encoded.cover.bc")
    add_custom_command(OUTPUT ${ENC_COVER_BC}
                       COMMAND ${OPT} -load ${NO_ENC_INLINE_LIB}
                               -NoEncInline ${ENC_BC}
                               -o ${ENC_COVER_BC}
                       DEPENDS ${ENC_BC})
    set(ENC_BC ${ENC_COVER_BC})
  endif(${COVER_TEST})
# Link both modules:
  add_custom_command(OUTPUT ${ENCODED_BC}
                     COMMAND ${LLVM_LINK}
                             -o ${ENCODED_BC}
                             ${MAIN_BC} ${ENC_BC}
                             ${BC_LIBS}/myargs.bc
                             ${BC_LIBS}/mycheck.bc
                             ${BC_LIBS}/mycyc.bc
                     DEPENDS ${MAIN_BC} ${ENC_BC} mycheck.bc mycyc.bc myargs.bc)
# Build the 'encoded' executable:
  add_custom_command(OUTPUT ${ENCODED_TARGET} ALL
                     COMMAND ${CLANG} ${CLANG_BACKEND_OPTS}
                             -o ${ENCODED_TARGET}
                             ${ENCODED_BC}
                     DEPENDS ${ENCODED_BC})
# Generate an assembly file (for debugging of the encoder):
  add_custom_command(OUTPUT ${ENCODED_S}
                     COMMAND ${CLANG} -S ${CLANG_BACKEND_OPTS}
                             -o ${ENCODED_S}
                             ${ENCODED_BC}
                     DEPENDS ${ENCODED_BC})
# HACK: If 'add_custom_target' is used for both '${ENCODED_TARGET}' and '${ENCODED_S}',
# then "make -jX" fails for 'X > 1'. Apparently this is a CMake problem  that is at
# least vaguley known to the relevant comunity. Here this problem is avoided by
# creating the dummy target '${TARGET}' and using 'add_custom_command' to generate
# both '${ENCODED_TARGET}' and '${ENCODED_S}':
  add_custom_target(${TARGET} ALL
                    DEPENDS ${ENCODED_TARGET} ${ENCODED_BC})
endfunction(BUILD_ENCODED_TEST)


