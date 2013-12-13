message("Running: \"${VERA_PROGRAM}\" \"--rule\" \"Broken\" \"--root\" \"/home/smartmatic/workspace/vera/tests/errorReport\" \"/home/smartmatic/workspace/vera/tests/test.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "--rule" "Broken" "--root" "/home/smartmatic/workspace/vera/tests/errorReport" "/home/smartmatic/workspace/vera/tests/test.cpp"
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
if(NOT "${error}" STREQUAL "vera++: can't read \"auie\": no such variable
    while executing
\"lindex $auie\"
    (procedure \"Foo\" line 2)
    invoked from within
\"Foo\"
    (procedure \"Bar\" line 2)
    invoked from within
\"Bar\"
    invoked from within
\"puts [Bar]\"
    (file \"/home/smartmatic/workspace/vera/tests/errorReport/scripts/rules/Broken.tcl\" line 9)
")
  message("Output mismatch. Expected content was:")
  message("vera++: can't read \"auie\": no such variable
    while executing
\"lindex $auie\"
    (procedure \"Foo\" line 2)
    invoked from within
\"Foo\"
    (procedure \"Bar\" line 2)
    invoked from within
\"Bar\"
    invoked from within
\"puts [Bar]\"
    (file \"/home/smartmatic/workspace/vera/tests/errorReport/scripts/rules/Broken.tcl\" line 9)
---")
  set(fail ON)
endif()

if(fail)
  message(FATAL_ERROR "some errors occurred.")
endif()
