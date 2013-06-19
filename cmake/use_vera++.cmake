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


macro(vera_incr var_name)
  math(EXPR ${var_name} "${${var_name}} + 1")
endmacro()

function(add_vera_targets)
  # default values
  set(target "style")
  set(target_all "style_reports")
  set(profile "default")
  set(root "${CMAKE_CURRENT_BINARY_DIR}")
  set(exclusions)
  set(recurse OFF)
  set(globs)
  # parse the options
  math(EXPR lastIdx "${ARGC} - 1")
  set(i 0)
  while(i LESS ${ARGC})
    set(arg "${ARGV${i}}")
    if("${arg}" STREQUAL "NAME")
      vera_incr(i)
      set(target "${ARGV${i}}")
    elseif("${arg}" STREQUAL "NAME_ALL")
      vera_incr(i)
      set(target_all "${ARGV${i}}")
    elseif("${arg}" STREQUAL "ROOT")
      vera_incr(i)
      set(root "${ARGV${i}}")
    elseif("${arg}" STREQUAL "PROFILE")
      vera_incr(i)
      set(profile "${ARGV${i}}")
    elseif("${arg}" STREQUAL "EXCLUSION")
      vera_incr(i)
      list(APPEND exclusions --exclusions "${ARGV${i}}")
    elseif("${arg}" STREQUAL "RECURSE")
      set(recurse ON)
    else()
      list(APPEND globs ${arg})
    endif()
    vera_incr(i)
  endwhile()

  if(recurse)
    file(GLOB_RECURSE srcs ${globs})
  else()
    file(GLOB srcs ${globs})
  endif()
  list(SORT srcs)

  if(NOT VERA++_EXECUTABLE AND TARGET vera)
    set(vera_program "$<TARGET_FILE:vera>")
  else()
    set(vera_program "${VERA++_EXECUTABLE}")
  endif()

  # Two custom targets will be created:
  # * style_reports is run as part of the build, and is not rerun unless one of
  # the file checked is modified;
  # * style must be explicitly called (make style) and is rerun even if the files
  # to check have not been modified. To achieve this behavior, the commands used
  # in this target pretend to produce a file without actually producing it.
  # Because the output file is not there after the run, the command will be rerun
  # again at the next target build.
  # The report style is selected based on the build environment, so the style
  # problems are properly reported in the IDEs
  if(MSVC)
    set(style vc)
  else()
    set(style std)
  endif()
  set(xmlreports)
  set(noreports)
  set(reportNb 0)
  set(reportsrcs)
  list(GET srcs 0 first)
  get_filename_component(currentDir ${first} PATH)
  # add a fake src file in a fake dir to trigger the creation of the last
  # custom command
  list(APPEND srcs "#12345678900987654321#/0987654321#1234567890")
  foreach(s ${srcs})
    get_filename_component(d ${s} PATH)
    if(NOT "${d}" STREQUAL "${currentDir}")
      # this is a new dir - lets generate everything needed for the previous dir
      string(LENGTH "${CMAKE_SOURCE_DIR}" len)
      string(SUBSTRING "${currentDir}" 0 ${len} pre)
      if("${pre}" STREQUAL "${CMAKE_SOURCE_DIR}")
        string(SUBSTRING "${currentDir}" ${len} -1 currentDir)
        string(REGEX REPLACE "^/" "" currentDir "${currentDir}")
      endif()
      if("${currentDir}" STREQUAL "")
        set(currentDir ".")
      endif()
      set(xmlreport ${CMAKE_CURRENT_BINARY_DIR}/vera_report_${reportNb}.xml)
      add_custom_command(
        OUTPUT ${xmlreport}
        COMMAND ${vera_program}
          --root ${root}
          --profile ${profile}
          --${style}-report=-
          --show-rule
          --warning
          --xml-report=${xmlreport}
          ${exclusions}
          ${reportsrcs}
        DEPENDS ${reportsrcs}
        COMMENT "Checking style with vera++ in ${currentDir}"
      )
      set(noreport ${CMAKE_CURRENT_BINARY_DIR}/vera_noreport_${reportNb}.xml)
      add_custom_command(
        OUTPUT ${noreport}
        COMMAND ${vera_program}
          --root ${root}
          --profile ${profile}
          --${style}-report=-
          --show-rule
          --warning
          # --xml-report=${noreport}
          ${exclusions}
          ${reportsrcs}
        DEPENDS ${reportsrcs}
        COMMENT "Checking style with vera++ in ${currentDir}"
      )
      list(APPEND xmlreports ${xmlreport})
      list(APPEND noreports ${noreport})
      vera_incr(reportNb)
      # clear the list for the next dir
      set(reportsrcs)
      set(currentDir ${d})
    endif()
    list(APPEND reportsrcs ${s})
  endforeach()
  # Create the custom targets that will trigger the custom command created
  # previously
  add_custom_target(${target_all} ALL DEPENDS ${xmlreports})
  add_custom_target(${target} DEPENDS ${noreports})
endfunction()
