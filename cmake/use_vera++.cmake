# remember where test_wrapper.cmake.in is
get_filename_component(VERA_TEST_WRAPPER_CMAKE
  ${CMAKE_CURRENT_LIST_FILE}/../test_wrapper.cmake.in ABSOLUTE)

function(vera_add_test_stdin_file name input output error retcode)
  if(NOT "${input}" STREQUAL "")
    set(input_option "INPUT_FILE \"${input}\"")
  else()
    set(input_option)
  endif()
  string(REPLACE ";" "\" \"" args "${ARGN}")
  set(args "\"${args}\"")
  string(REPLACE "\"" "\\\"" protected_args "${args}")
  string(REPLACE "\\" "\\\\" protected_output "${output}")
  string(REPLACE "\"" "\\\"" protected_output "${protected_output}")
  string(REPLACE "\\" "\\\\" protected_error "${error}")
  string(REPLACE "\"" "\\\"" protected_error "${protected_error}")
  configure_file(${VERA_TEST_WRAPPER_CMAKE}
    ${CMAKE_CURRENT_BINARY_DIR}/${name}.cmake @ONLY)

  if(NOT VERA++_EXECUTABLE AND TARGET vera)
    set(vera_program "$<TARGET_FILE:vera>")
  else()
    set(vera_program "${VERA++_EXECUTABLE}")
  endif()
  add_test(NAME ${name}
    COMMAND ${CMAKE_COMMAND}
    -D VERA_PROGRAM:FILEPATH=${vera_program}
    -P ${CMAKE_CURRENT_BINARY_DIR}/${name}.cmake)
endfunction()

function(vera_add_test name input output error retcode)
  if(NOT "${input}" STREQUAL "")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${name}-input.txt" "${input}")
    set(input_file "${CMAKE_CURRENT_BINARY_DIR}/${name}-input.txt")
  else()
    set(input_file)
  endif()
  vera_add_test_stdin_file("${name}" "${input_file}" "${output}" "${error}" "${retcode}" ${ARGN})
endfunction()

function(vera_add_rule_test name output)
  if(VERA_TEST_RULE_ROOT)
    set(root ${VERA_TEST_RULE_ROOT})
  else()
    set(root ${CMAKE_CURRENT_SOURCE_DIR})
  endif()
  set(full_input "${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp")
  string(REPLACE "\n" "\n${full_input}:" formated_output "${output}")
  set(formated_output "${full_input}:${formated_output}\n")
  vera_add_test(Rule${name} "" "${formated_output}" "" 0
    --root ${root}
    --rule ${name}
    ${full_input}
  )
endfunction()
