message("Running: \"${VERA_PROGRAM}\" \"--root\" \"/home/smartmatic/workspace/vera\" \"--rule\" \"T009\" \"-d\" \"/home/smartmatic/workspace/vera/tests/T009.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "--root" "/home/smartmatic/workspace/vera" "--rule" "T009" "-d" "/home/smartmatic/workspace/vera/tests/T009.cpp"
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
if(NOT "${output}" STREQUAL "/home/smartmatic/workspace/vera/tests/T009.cpp:10: comma should not be preceded by whitespace
/home/smartmatic/workspace/vera/tests/T009.cpp:12: comma should not be preceded by whitespace
/home/smartmatic/workspace/vera/tests/T009.cpp:13: comma should not be preceded by whitespace
/home/smartmatic/workspace/vera/tests/T009.cpp:14: comma should not be preceded by whitespace
")
  message("Output mismatch. Expected content was:")
  message("/home/smartmatic/workspace/vera/tests/T009.cpp:10: comma should not be preceded by whitespace
/home/smartmatic/workspace/vera/tests/T009.cpp:12: comma should not be preceded by whitespace
/home/smartmatic/workspace/vera/tests/T009.cpp:13: comma should not be preceded by whitespace
/home/smartmatic/workspace/vera/tests/T009.cpp:14: comma should not be preceded by whitespace
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
