message("Running: \"${VERA_PROGRAM}\" \"--root\" \"/home/smartmatic/workspace/vera\" \"--rule\" \"T004\" \"/home/smartmatic/workspace/vera/tests/T004.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "--root" "/home/smartmatic/workspace/vera" "--rule" "T004" "/home/smartmatic/workspace/vera/tests/T004.cpp"
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
if(NOT "${output}" STREQUAL "/home/smartmatic/workspace/vera/tests/T004.cpp:25: colon not immediately after the 'public' keyword
/home/smartmatic/workspace/vera/tests/T004.cpp:28: colon not immediately after the 'protected' keyword
/home/smartmatic/workspace/vera/tests/T004.cpp:30: colon not immediately after the 'private' keyword
/home/smartmatic/workspace/vera/tests/T004.cpp:39: colon not immediately after the 'default' keyword
")
  message("Output mismatch. Expected content was:")
  message("/home/smartmatic/workspace/vera/tests/T004.cpp:25: colon not immediately after the 'public' keyword
/home/smartmatic/workspace/vera/tests/T004.cpp:28: colon not immediately after the 'protected' keyword
/home/smartmatic/workspace/vera/tests/T004.cpp:30: colon not immediately after the 'private' keyword
/home/smartmatic/workspace/vera/tests/T004.cpp:39: colon not immediately after the 'default' keyword
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
