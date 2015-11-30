cmake_minimum_required(VERSION 2.8)


# Require LLVM with builtins/intrinsics for encoding:
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")


# Define short-hands for a couple of LLVM's binaries:
set(CLANG     ${LLVM_INSTALL_PREFIX}/bin/clang)
set(CLANGXX   ${LLVM_INSTALL_PREFIX}/bin/clang++)
set(LLVM_LINK ${LLVM_INSTALL_PREFIX}/bin/llvm-link)
set(LLVM_DIS  ${LLVM_INSTALL_PREFIX}/bin/llvm-dis)
set(OPT       ${LLVM_INSTALL_PREFIX}/bin/opt)


# Use 'clang' as the standard compiler:
set(CMAKE_C_COMPILER ${CLANG})
set(CMAKE_CXX_COMPILER ${CLANGXX})



# Library of functions common to all test cases:
add_subdirectory(utility)


# If desired, build the configuration files first ...
set(BUILD_CONFIGS FALSE CACHE BOOL "")
if(BUILD_CONFIGS)
# ... because we want 'BINARIES_FOLDER' set when we get to the
# configuration files for the inddividual test cases below.
  set(BINARIES_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/tests)
  add_subdirectory(configs)
endif(BUILD_CONFIGS)


# Build the test cases:
add_subdirectory(tests)


