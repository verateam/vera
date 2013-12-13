message("Running: \"${VERA_PROGRAM}\" \"--root\" \"/home/smartmatic/workspace/vera\" \"--rule\" \"T019\" \"/home/smartmatic/workspace/vera/tests/T019.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "--root" "/home/smartmatic/workspace/vera" "--rule" "T019" "/home/smartmatic/workspace/vera/tests/T019.cpp"
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
if(NOT "${output}" STREQUAL "/home/smartmatic/workspace/vera/tests/T019.cpp:1: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:5: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:17: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:33: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:41: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:42: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:52: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:61: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:62: full block {} expected in the control structure
")
  message("Output mismatch. Expected content was:")
  message("/home/smartmatic/workspace/vera/tests/T019.cpp:1: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:5: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:17: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:33: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:41: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:42: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:52: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:61: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/T019.cpp:62: full block {} expected in the control structure
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
