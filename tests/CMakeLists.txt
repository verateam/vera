# just to show a way to use vera++
file(GLOB_RECURSE srcs ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.h)
add_test(NAME VeraStyle
  COMMAND vera
  --error
  --root "${CMAKE_SOURCE_DIR}"
  ${srcs}
)

# now the real tests

include(utils.cmake)

####### command line iterface tests #######

vera_add_test(NoRoot
  "" "" "error: cannot open profile description for profile default\n" 1
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp"
)

vera_add_test(StdInNoArgs
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp\n"
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no copyright notice found\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
)

vera_add_test_stdin_file(StdInDash
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp"
  "-:1: no copyright notice found\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR};-"
)

vera_add_test(VCReport
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp(1): no copyright notice found\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --vc-report=-
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(ShowRule
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: T013: no copyright notice found\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --show-rule
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(Warning
  "" ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: warning: no copyright notice found\n"
  0
  --root "${CMAKE_SOURCE_DIR}"
  --warning
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(WarningShowRule
  "" ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: warning T013: no copyright notice found\n"
  0
  --root "${CMAKE_SOURCE_DIR}"
  --warning
  --show-rule
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(Error
  "" ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: error: no copyright notice found\n"
  1
  --root "${CMAKE_SOURCE_DIR}"
  --error
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(ErrorShowRule
  "" ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: error T013: no copyright notice found\n"
  1
  --root "${CMAKE_SOURCE_DIR}"
  --error
  --show-rule
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)


####### rule tests #######

vera_add_rule_test(T013 "test.cpp" "1: no copyright notice found")

