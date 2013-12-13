message("Running: \"${VERA_PROGRAM}\" \"--root\" \"/home/smartmatic/workspace/vera/tests/python\" \"--rule\" \"T019.py\" \"/home/smartmatic/workspace/vera/tests/python/T019.py.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "--root" "/home/smartmatic/workspace/vera/tests/python" "--rule" "T019.py" "/home/smartmatic/workspace/vera/tests/python/T019.py.cpp"
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
if(NOT "${output}" STREQUAL "/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:1: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:5: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:17: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:33: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:41: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:42: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:52: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:61: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:62: full block {} expected in the control structure
")
  message("Output mismatch. Expected content was:")
  message("/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:1: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:5: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:17: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:33: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:41: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:42: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:52: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:61: full block {} expected in the control structure
/home/smartmatic/workspace/vera/tests/python/T019.py.cpp:62: full block {} expected in the control structure
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
