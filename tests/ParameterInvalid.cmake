message("Running: \"${VERA_PROGRAM}\" \"--root\" \"/home/smartmatic/workspace/vera\" \"--rule\" \"L004\" \"--parameter\" \"max-line-length\" \"/home/smartmatic/workspace/vera/tests/L004.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "--root" "/home/smartmatic/workspace/vera" "--rule" "L004" "--parameter" "max-line-length" "/home/smartmatic/workspace/vera/tests/L004.cpp"
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
if(NOT "${error}" STREQUAL "vera++: Invalid parameter association: max-line-length
")
  message("Output mismatch. Expected content was:")
  message("vera++: Invalid parameter association: max-line-length
---")
  set(fail ON)
endif()

if(fail)
  message(FATAL_ERROR "some errors occurred.")
endif()
