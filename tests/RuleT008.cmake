message("Running: \"${VERA_PROGRAM}\" \"--root\" \"/home/smartmatic/workspace/vera\" \"--rule\" \"T008\" \"/home/smartmatic/workspace/vera/tests/T008.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "--root" "/home/smartmatic/workspace/vera" "--rule" "T008" "/home/smartmatic/workspace/vera/tests/T008.cpp"
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
if(NOT "${output}" STREQUAL "/home/smartmatic/workspace/vera/tests/T008.cpp:22: keyword 'catch' not followed by a single space
/home/smartmatic/workspace/vera/tests/T008.cpp:24: keyword 'for' not followed by a single space
/home/smartmatic/workspace/vera/tests/T008.cpp:26: keyword 'if' not followed by a single space
/home/smartmatic/workspace/vera/tests/T008.cpp:28: keyword 'while' not followed by a single space
")
  message("Output mismatch. Expected content was:")
  message("/home/smartmatic/workspace/vera/tests/T008.cpp:22: keyword 'catch' not followed by a single space
/home/smartmatic/workspace/vera/tests/T008.cpp:24: keyword 'for' not followed by a single space
/home/smartmatic/workspace/vera/tests/T008.cpp:26: keyword 'if' not followed by a single space
/home/smartmatic/workspace/vera/tests/T008.cpp:28: keyword 'while' not followed by a single space
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
