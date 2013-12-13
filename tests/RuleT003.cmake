message("Running: \"${VERA_PROGRAM}\" \"--root\" \"/home/smartmatic/workspace/vera\" \"--rule\" \"T003\" \"/home/smartmatic/workspace/vera/tests/T003.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "--root" "/home/smartmatic/workspace/vera" "--rule" "T003" "/home/smartmatic/workspace/vera/tests/T003.cpp"
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
if(NOT "${output}" STREQUAL "/home/smartmatic/workspace/vera/tests/T003.cpp:1: keyword 'case' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:2: keyword 'class' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:3: keyword 'delete' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:4: keyword 'enum' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:5: keyword 'explicit' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:6: keyword 'extern' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:7: keyword 'goto' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:8: keyword 'new' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:9: keyword 'struct' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:10: keyword 'union' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:11: keyword 'using' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:13: keyword 'case' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:14: keyword 'class' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:15: keyword 'delete' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:16: keyword 'enum' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:17: keyword 'explicit' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:18: keyword 'extern' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:19: keyword 'goto' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:20: keyword 'new' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:21: keyword 'struct' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:22: keyword 'union' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:23: keyword 'using' not followed by a single space
")
  message("Output mismatch. Expected content was:")
  message("/home/smartmatic/workspace/vera/tests/T003.cpp:1: keyword 'case' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:2: keyword 'class' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:3: keyword 'delete' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:4: keyword 'enum' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:5: keyword 'explicit' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:6: keyword 'extern' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:7: keyword 'goto' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:8: keyword 'new' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:9: keyword 'struct' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:10: keyword 'union' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:11: keyword 'using' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:13: keyword 'case' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:14: keyword 'class' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:15: keyword 'delete' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:16: keyword 'enum' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:17: keyword 'explicit' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:18: keyword 'extern' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:19: keyword 'goto' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:20: keyword 'new' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:21: keyword 'struct' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:22: keyword 'union' not followed by a single space
/home/smartmatic/workspace/vera/tests/T003.cpp:23: keyword 'using' not followed by a single space
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
