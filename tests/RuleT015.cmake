message("Running: \"${VERA_PROGRAM}\" \"--root\" \"/home/smartmatic/workspace/vera\" \"--rule\" \"T015\" \"/home/smartmatic/workspace/vera/tests/T015.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "--root" "/home/smartmatic/workspace/vera" "--rule" "T015" "/home/smartmatic/workspace/vera/tests/T015.cpp"
  RESULT_VARIABLE ret
  OUTPUT_VARIABLE output
  ERROR_VARIABLE error
  )
message("return value: ${ret}")
message("---output---\n${output}---error----\n${error}------------")

set(fail OFF)
if(NOT ${ret} EQUAL 0)
  message("Invalid return code. Expected return code was 0.")
  set(fail ON)
endif()
if(NOT "${output}" STREQUAL "/home/smartmatic/workspace/vera/tests/T015.cpp:8: URL points to non-existing file
/home/smartmatic/workspace/vera/tests/T015.cpp:9: URL points to non-existing file
")
  message("Output mismatch. Expected content was:")
  message("/home/smartmatic/workspace/vera/tests/T015.cpp:8: URL points to non-existing file
/home/smartmatic/workspace/vera/tests/T015.cpp:9: URL points to non-existing file
---")
  set(fail ON)
endif()
if(NOT "${error}" STREQUAL "")
  message("Output mismatch. Expected content was:")
  message("---")
  set(fail ON)
endif()

if(fail)
  message(FATAL_ERROR "some errors occurred.")
endif()
