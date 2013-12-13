message("Running: \"${VERA_PROGRAM}\" \"--rule\" \"T014\" \"--root\" \"/home/smartmatic/workspace/vera\" \"/home/smartmatic/workspace/vera/tests/test.cpp\" \"/home/smartmatic/workspace/vera/tests/T013.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "--rule" "T014" "--root" "/home/smartmatic/workspace/vera" "/home/smartmatic/workspace/vera/tests/test.cpp" "/home/smartmatic/workspace/vera/tests/T013.cpp"
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
if(NOT "${output}" STREQUAL "/home/smartmatic/workspace/vera/tests/test.cpp:1: no reference to the Boost Software License found
")
  message("Output mismatch. Expected content was:")
  message("/home/smartmatic/workspace/vera/tests/test.cpp:1: no reference to the Boost Software License found
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
