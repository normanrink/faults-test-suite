cmake_minimum_required(VERSION 2.8)


if(NOT DEFINED PIN_COMMAND OR NOT PIN_COMMAND)
  message(FATAL_ERROR "'PIN_COMMAND' not set.")
endif(NOT DEFINED PIN_COMMAND OR NOT PIN_COMMAND)

if(NOT DEFINED BFI_SO_PATH OR NOT BFI_SO_PATH)
  message(FATAL_ERROR "'BFI_SO_PATH' not set.")
endif(NOT DEFINED BFI_SO_PATH OR NOT BFI_SO_PATH)

if(NOT DEFINED SUITE_FOLDER OR NOT SUITE_FOLDER)
  message(FATAL_ERROR "'SUITE_FOLDER' not set.")
endif(NOT DEFINED SUITE_FOLDER OR NOT SUITE_FOLDER)

if(NOT DEFINED WARM_UP_RUNS OR NOT WARM_UP_RUNS)
  message(FATAL_ERROR "'WARM_UP_RUNS' not set.")
endif(NOT DEFINED WARM_UP_RUNS OR NOT WARM_UP_RUNS)

if(NOT DEFINED FI_RUNS OR NOT FI_RUNS)
  message(FATAL_ERROR "'FI_RUNS' not set.")
endif(NOT DEFINED FI_RUNS OR NOT FI_RUNS)
