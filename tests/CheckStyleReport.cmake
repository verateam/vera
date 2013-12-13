message("Running: \"${VERA_PROGRAM}\" \"--root\" \"/home/smartmatic/workspace/vera\" \"--checkstyle-report=-\" \"/home/smartmatic/workspace/vera/tests/test.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "--root" "/home/smartmatic/workspace/vera" "--checkstyle-report=-" "/home/smartmatic/workspace/vera/tests/test.cpp"
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
if(NOT "${output}" STREQUAL "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<checkstyle version=\"5.0\">
    <file name=\"/home/smartmatic/workspace/vera/tests/test.cpp\">
        <error source=\"T013\" severity=\"info\" line=\"1\" message=\"no copyright notice found\" />
    </file>
</checkstyle>
")
  message("Output mismatch. Expected content was:")
  message("<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<checkstyle version=\"5.0\">
    <file name=\"/home/smartmatic/workspace/vera/tests/test.cpp\">
        <error source=\"T013\" severity=\"info\" line=\"1\" message=\"no copyright notice found\" />
    </file>
</checkstyle>
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
