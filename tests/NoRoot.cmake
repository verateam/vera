message("Running: \"${VERA_PROGRAM}\" \"/home/smartmatic/workspace/vera/tests/test.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "/home/smartmatic/workspace/vera/tests/test.cpp"
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
if(NOT "${error}" STREQUAL "vera++: Cannot open profile description for profile 'default': No such file or directory
")
  message("Output mismatch. Expected content was:")
  message("vera++: Cannot open profile description for profile 'default': No such file or directory
---")
  set(fail ON)
endif()

if(fail)
  message(FATAL_ERROR "some errors occurred.")
endif()
