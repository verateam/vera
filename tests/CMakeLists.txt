add_test(NAME VeraNoRootKO
  COMMAND vera++
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")
set_tests_properties(VeraNoRootKO PROPERTIES WILL_FAIL ON)

add_test(NAME VeraRoot
  COMMAND vera++
  --root "${CMAKE_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")
set_tests_properties(VeraRoot PROPERTIES
  PASS_REGULAR_EXPRESSION "test.cpp:1: no copyright notice found")

add_test(NAME VeraRootOK
  COMMAND vera++
  --root "${CMAKE_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")

add_test(NAME VeraVCFormat
  COMMAND vera++
  --root "${CMAKE_SOURCE_DIR}"
  --vc-report=-
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")
set_tests_properties(VeraVCFormat PROPERTIES
  PASS_REGULAR_EXPRESSION "test.cpp\\(1\\): no copyright notice found")

add_test(NAME VeraVCFormatOK
  COMMAND vera++
  --root "${CMAKE_SOURCE_DIR}"
  --vc-report=-
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")

add_test(NAME VeraShowRule
  COMMAND vera++
  --root "${CMAKE_SOURCE_DIR}"
  --show-rule
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")
set_tests_properties(VeraShowRule PROPERTIES
  PASS_REGULAR_EXPRESSION "test.cpp:1: T013: no copyright notice found")

add_test(NAME VeraShowRuleOK
  COMMAND vera++
  --root "${CMAKE_SOURCE_DIR}"
  --show-rule
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")

add_test(NAME VeraWarning
  COMMAND vera++
  --root "${CMAKE_SOURCE_DIR}"
  --warning
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")
set_tests_properties(VeraWarning PROPERTIES
  PASS_REGULAR_EXPRESSION "test.cpp:1: warning: no copyright notice found")

# make sure the return value is 0 - can't be tested at the same time than the
# regular expression
add_test(NAME VeraWarningOK
  COMMAND vera++
  --root "${CMAKE_SOURCE_DIR}"
  --warning
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")

add_test(NAME VeraWarningShowRule
  COMMAND vera++
  --root "${CMAKE_SOURCE_DIR}"
  --warning
  --show-rule
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")
set_tests_properties(VeraWarningShowRule PROPERTIES
  PASS_REGULAR_EXPRESSION "test.cpp:1: warning T013: no copyright notice found")

add_test(NAME VeraWarningShowRuleOK
  COMMAND vera++
  --root "${CMAKE_SOURCE_DIR}"
  --warning
  --show-rule
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")

add_test(NAME VeraError
  COMMAND vera++
  --root "${CMAKE_SOURCE_DIR}"
  --error
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")
set_tests_properties(VeraError PROPERTIES
  PASS_REGULAR_EXPRESSION "test.cpp:1: error: no copyright notice found")

add_test(NAME VeraErrorKO
  COMMAND vera++
  --root "${CMAKE_SOURCE_DIR}"
  --error
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")
set_tests_properties(VeraErrorKO PROPERTIES WILL_FAIL ON)

add_test(NAME VeraErrorShowRule
  COMMAND vera++
  --root "${CMAKE_SOURCE_DIR}"
  --error
  --show-rule
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")
set_tests_properties(VeraErrorShowRule PROPERTIES
  PASS_REGULAR_EXPRESSION "test.cpp:1: error T013: no copyright notice found")

add_test(NAME VeraErrorShowRuleKO
  COMMAND vera++
  --root "${CMAKE_SOURCE_DIR}"
  --error
  --show-rule
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")
set_tests_properties(VeraErrorShowRuleKO PROPERTIES WILL_FAIL ON)
