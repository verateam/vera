message("Running: \"${VERA_PROGRAM}\" \"--root\" \"/home/smartmatic/workspace/vera\" \"--rule\" \"T011\" \"/home/smartmatic/workspace/vera/tests/T011.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "--root" "/home/smartmatic/workspace/vera" "--rule" "T011" "/home/smartmatic/workspace/vera/tests/T011.cpp"
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
if(NOT "${output}" STREQUAL "/home/smartmatic/workspace/vera/tests/T011.cpp:22: closing curly bracket not in the same line or column
/home/smartmatic/workspace/vera/tests/T011.cpp:27: closing curly bracket not in the same line or column
/home/smartmatic/workspace/vera/tests/T011.cpp:30: closing curly bracket not in the same line or column
/home/smartmatic/workspace/vera/tests/T011.cpp:39: closing curly bracket not in the same line or column
")
  message("Output mismatch. Expected content was:")
  message("/home/smartmatic/workspace/vera/tests/T011.cpp:22: closing curly bracket not in the same line or column
/home/smartmatic/workspace/vera/tests/T011.cpp:27: closing curly bracket not in the same line or column
/home/smartmatic/workspace/vera/tests/T011.cpp:30: closing curly bracket not in the same line or column
/home/smartmatic/workspace/vera/tests/T011.cpp:39: closing curly bracket not in the same line or column
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
