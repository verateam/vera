message("Running: \"${VERA_PROGRAM}\" \"-eSqpdefault\" \"-r\" \"/home/smartmatic/workspace/vera\" \"/home/smartmatic/workspace/vera/tests/test.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "-eSqpdefault" "-r" "/home/smartmatic/workspace/vera" "/home/smartmatic/workspace/vera/tests/test.cpp"
  RESULT_VARIABLE ret
  OUTPUT_VARIABLE output
  ERROR_VARIABLE error
  )
message("return value: ${ret}")
message("---output---\n${output}---error----\n${error}------------")

set(fail OFF)
if(NOT ${ret} EQUAL 1)
  message("Invalid return code. Expected return code was 1.")
  set(fail ON)
endif()
if(NOT "${output}" STREQUAL "")
  message("Output mismatch. Expected content was:")
  message("---")
  set(fail ON)
endif()
if(NOT "${error}" STREQUAL "1 reports in 1 files.
")
  message("Output mismatch. Expected content was:")
  message("1 reports in 1 files.
---")
  set(fail ON)
endif()

if(fail)
  message(FATAL_ERROR "some errors occurred.")
endif()
