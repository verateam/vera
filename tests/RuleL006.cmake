message("Running: \"${VERA_PROGRAM}\" \"--rule\" \"L006\" \"--root\" \"/home/smartmatic/workspace/vera\" \"/home/smartmatic/workspace/vera/tests/L006-1.cpp\" \"/home/smartmatic/workspace/vera/tests/L006-2.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "--rule" "L006" "--root" "/home/smartmatic/workspace/vera" "/home/smartmatic/workspace/vera/tests/L006-1.cpp" "/home/smartmatic/workspace/vera/tests/L006-2.cpp"
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
if(NOT "${output}" STREQUAL "/home/smartmatic/workspace/vera/tests/L006-2.cpp:2001: source file is too long
")
  message("Output mismatch. Expected content was:")
  message("/home/smartmatic/workspace/vera/tests/L006-2.cpp:2001: source file is too long
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
