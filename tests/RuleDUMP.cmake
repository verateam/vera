message("Running: \"${VERA_PROGRAM}\" \"--rule\" \"DUMP\" \"--root\" \"/home/smartmatic/workspace/vera\" \"/home/smartmatic/workspace/vera/tests/T017.cpp\"")
execute_process(COMMAND "${VERA_PROGRAM}"
  "--rule" "DUMP" "--root" "/home/smartmatic/workspace/vera" "/home/smartmatic/workspace/vera/tests/T017.cpp"
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
if(NOT "${output}" STREQUAL "Tokens in file /home/smartmatic/workspace/vera/tests/T017.cpp:
1/0	namespace	namespace
1/9	newline	

2/0	leftbrace	{
2/1	newline	

3/0	cppcomment	// some stuff here

4/0	rightbrace	}
4/1	newline	

5/0	eof	

")
  message("Output mismatch. Expected content was:")
  message("Tokens in file /home/smartmatic/workspace/vera/tests/T017.cpp:
1/0	namespace	namespace
1/9	newline	

2/0	leftbrace	{
2/1	newline	

3/0	cppcomment	// some stuff here

4/0	rightbrace	}
4/1	newline	

5/0	eof	

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
