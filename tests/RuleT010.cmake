message("Running: \"${VERA_PROGRAM}\" \"--root\" \"/home/smartmatic/workspace/vera\" \"--rule\" \"T010\" \"/home/smartmatic/workspace/vera/tests/T010.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "--root" "/home/smartmatic/workspace/vera" "--rule" "T010" "/home/smartmatic/workspace/vera/tests/T010.cpp"
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
if(NOT "${output}" STREQUAL "/home/smartmatic/workspace/vera/tests/T010.cpp:5: identifier should not be composed of only 'l' and 'O'
/home/smartmatic/workspace/vera/tests/T010.cpp:6: identifier should not be composed of only 'l' and 'O'
")
  message("Output mismatch. Expected content was:")
  message("/home/smartmatic/workspace/vera/tests/T010.cpp:5: identifier should not be composed of only 'l' and 'O'
/home/smartmatic/workspace/vera/tests/T010.cpp:6: identifier should not be composed of only 'l' and 'O'
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
