cmake_minimum_required(VERSION 2.8)


if(NOT DEFINED ENCODER OR NOT ENCODER)
  message(FATAL_ERROR "'ENCODER' not set")
endif(NOT DEFINED ENCODER OR NOT ENCODER)


# Helper function for cleaning ip "CUSTOM_TARGETs":
function(ADD_MAKE_CLEAN_FILES FILES)
  get_directory_property(FILES_TO_CLEAN ADDITIONAL_MAKE_CLEAN_FILES)
  list(APPEND FILES_TO_CLEAN ${FILES})
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
                                      "${FILES_TO_CLEAN}")
endfunction(ADD_MAKE_CLEAN_FILES)


function(ADD_BC_TARGET TARGET SOURCE FLAGS)
  add_custom_target(${TARGET} ALL
                    COMMAND ${CLANG} -c -emit-llvm ${FLAGS}
                            -o ${TARGET}
                            ${SOURCE}
                    DEPENDS ${SOURCE})
  # "CUSTOM_TARGETs" are not usually removed by 'make clean'.
  # Hence add this target to the list of files to be cleaned:
  ADD_MAKE_CLEAN_FILES(${TARGET})
endfunction(ADD_BC_TARGET)


function(ADD_LL_TARGET TARGET SOURCE FLAGS)
  add_custom_target(${TARGET} ALL
                     COMMAND ${CLANG} -S -emit-llvm ${FLAGS}
                             -o ${TARGET}
                             ${SOURCE}
                     DEPENDS ${SOURCE})
  # "CUSTOM_TARGETs" are not usually removed by 'make clean'.
  # Hence add this target to the list of files to be cleaned:
  ADD_MAKE_CLEAN_FILES(${TARGET})
endfunction(ADD_LL_TARGET)


function(ADD_ENCODED_BC_TARGET TARGET SOURCE FLAGS)
  add_custom_target(${TARGET} ALL
                    COMMAND ${ENCODER} ${FLAGS}
                            -o ${TARGET}
                            ${SOURCE}
                    DEPENDS ${SOURCE})
  # "CUSTOM_TARGETs" are not usually removed by 'make clean'.
  # Hence add this target to the list of files to be cleaned:
  ADD_MAKE_CLEAN_FILES(${TARGET})
endfunction(ADD_ENCODED_BC_TARGET)


function(ADD_BIN_TARGET TARGET SOURCE FLAGS LINK_FLAGS DEPENDENCIES)
  add_custom_target(${TARGET} ALL
                    COMMAND ${CLANG} ${FLAGS}
                            -o ${TARGET}
                             ${SOURCE}
                            ${LINK_FLAGS}
                    DEPENDS ${SOURCE}
                            ${DEPENDENCIES})
  # "CUSTOM_TARGETs" are not usually removed by 'make clean'.
  # Hence add this target to the list of files to be cleaned:
  ADD_MAKE_CLEAN_FILES(${TARGET})
endfunction(ADD_BIN_TARGET)


function(ADD_ASM_TARGET TARGET SOURCE FLAGS LINK_FLAGS DEPENDENCIES)
  add_custom_command(OUTPUT ${TARGET}
                     COMMAND ${CLANG} -S ${FLAGS}
                             -o ${TARGET}
                             ${SOURCE}
                             ${LINK_FLAGS}
                     DEPENDS ${SOURCE}
                             ${DEPENDENCIES})
  # "CUSTOM_TARGETs" are not usually removed by 'make clean'.
  # Hence add this target to the list of files to be cleaned:
  ADD_MAKE_CLEAN_FILES(${TARGET})
endfunction(ADD_ASM_TARGET)


function(ADD_LL_DIS_TARGET TARGET SOURCE)
  add_custom_target(${TARGET} ALL
                    COMMAND ${LLVM_DIS}
                            ${SOURCE}
                            -o ${TARGET}
                    DEPENDS ${SOURCE})
  # "CUSTOM_TARGETs" are not usually removed by 'make clean'.
  # Hence add this target to the list of files to be cleaned:
  ADD_MAKE_CLEAN_FILES(${TARGET})
endfunction(ADD_LL_DIS_TARGET)


function(ADD_DISASM_TARGET TARGET SOURCE)
  add_custom_target(${TARGET} ALL
                    COMMAND ${OBJDUMP}
                            -D ${SOURCE}
                            "1>" ${TARGET}
                    DEPENDS ${SOURCE})
  # "CUSTOM_TARGETs" are not usually removed by 'make clean'.
  # Hence add this target to the list of files to be cleaned:
  ADD_MAKE_CLEAN_FILES(${TARGET})
endfunction(ADD_DISASM_TARGET)


function(ADD_OUTPUT_TARGET TARGET CMD DEPENDENCIES)
  add_custom_target(${TARGET} ALL
                    COMMAND ${CMD}
                    DEPENDS ${DEPENDENCIES})
  # "CUSTOM_TARGETs" are not usually removed by 'make clean'.
  # Hence add this target to the list of files to be cleaned:
  ADD_MAKE_CLEAN_FILES(${TARGET})
endfunction(ADD_OUTPUT_TARGET)



