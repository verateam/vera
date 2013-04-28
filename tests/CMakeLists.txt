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

# this test is supposed to not found the vera root dir and fail because of that,
# but it will found one if vera has already been installed, so we don't run
# it in that case.
if(NOT EXISTS "${CMAKE_INSTALL_PREFIX}/lib/vera++/")
  vera_add_test(NoRoot
    "" "" "error: cannot open profile description for profile default\n" 1
    "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp"
  )
endif()

vera_add_test(VeraRootEnvironment
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no copyright notice found\n"
  "" 0
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)
set_tests_properties(VeraRootEnvironment
  PROPERTIES ENVIRONMENT "VERA_ROOT=${CMAKE_SOURCE_DIR}")

vera_add_test(VeraRootCurrentDirectory
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no copyright notice found\n"
  "" 0
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)
set_tests_properties(VeraRootCurrentDirectory
  PROPERTIES WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

vera_add_test(VeraLegacy
  "" ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no copyright notice found\n"
  0
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)
set_tests_properties(VeraLegacy
  PROPERTIES ENVIRONMENT "VERA_LEGACY=ON;VERA_ROOT=${CMAKE_SOURCE_DIR}")

vera_add_test(StdInNoArgs
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp\n"
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no copyright notice found\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
)

vera_add_test(Input
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp\n"
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no copyright notice found\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --input=-
)

vera_add_test(ShortInput
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp\n"
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no copyright notice found\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  -i -
)

vera_add_test_stdin_file(StdInDash
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp"
  "-:1: no copyright notice found\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  -
)

vera_add_test(StandardReport
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no copyright notice found\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --std-report=-
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(ShortStandardReport
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no copyright notice found\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  -o -
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(VCReport
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp(1): no copyright notice found\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --vc-report=-
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(ShortVCReport
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp(1): no copyright notice found\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  -v -
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(XMLReport
  ""
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<vera>
    <file name=\"${CMAKE_CURRENT_SOURCE_DIR}/test.cpp\">
        <report line=\"1\">![CDATA[no copyright notice found]]</report>
    </file>
</vera>\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --xml-report=-
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(ShortXMLReport
  ""
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<vera>
    <file name=\"${CMAKE_CURRENT_SOURCE_DIR}/test.cpp\">
        <report line=\"1\">![CDATA[no copyright notice found]]</report>
    </file>
</vera>\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  -x -
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

vera_add_test(ShortShowRule
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: T013: no copyright notice found\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  -s
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

vera_add_test(ShortWarning
  "" ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: warning: no copyright notice found\n"
  0
  --root "${CMAKE_SOURCE_DIR}"
  -w
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

vera_add_test(Quiet
  "" "" "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --quiet
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(ShortQuiet
  "" "" "" 0
  --root "${CMAKE_SOURCE_DIR}"
  -q
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(Summary
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no copyright notice found\n"
  "1 reports in 1 files.\n" 0
  --root "${CMAKE_SOURCE_DIR}"
  --summary
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(ShortSummary
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no copyright notice found\n"
  "1 reports in 1 files.\n" 0
  --root "${CMAKE_SOURCE_DIR}"
  -S
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(Version
  "" "${VERA_VERSION}\n" "" 0
  --version
)

vera_add_test(VersionLegacy
  "" "${VERA_VERSION}\n" "" 0
  -version
)

vera_add_test(AggregatedShortOptions
  "" "" "1 reports in 1 files.\n" 1
  -eSqpdefault
  -r "${CMAKE_SOURCE_DIR}"
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)


# options to test
# -p [ --profile ] arg
# -R [ --rule ] arg
# --transform arg
# -d [ --no-duplicate ]
# --parameters arg
# -P [ --param ] arg
# --exclusions arg
# -h [ --help ]

####### rule tests #######

vera_add_rule_test(F001 "4: \\r (CR) detected in isolation at position 7")

# F002

vera_add_rule_test(L001 "4: trailing whitespace
6: trailing whitespace")

vera_add_rule_test(L002 "2: horizontal tab used
4: horizontal tab used
6: horizontal tab used")

vera_add_test(RuleL003
  "" "${CMAKE_CURRENT_SOURCE_DIR}/L003-1.cpp:1: leading empty line(s)
${CMAKE_CURRENT_SOURCE_DIR}/L003-2.cpp:4: trailing empty line(s)\n"
  "" 0
  --rule L003
  --root "${CMAKE_SOURCE_DIR}"
  ${CMAKE_CURRENT_SOURCE_DIR}/L003-1.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/L003-2.cpp
)

vera_add_rule_test(L004 "1: line is longer than 100 characters
2: line is longer than 100 characters")

vera_add_rule_test(L005 "6: too many consecutive empty lines")

vera_add_test(RuleL006
  "" "${CMAKE_CURRENT_SOURCE_DIR}/L006-2.cpp:2001: source file is too long\n"
  "" 0
  --rule L006
  --root "${CMAKE_SOURCE_DIR}"
  ${CMAKE_CURRENT_SOURCE_DIR}/L006-1.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/L006-2.cpp
)

vera_add_rule_test(T001 "6: line-continuation in one-line comment")

set(output "4: reserved name used for macro (incorrect use of underscore)
5: reserved name used for macro (incorrect use of underscore)")
foreach(i RANGE 7 80)
  set(output "${output}\n${i}: reserved name used for macro (keyword or alternative token redefined)")
endforeach()
vera_add_rule_test(T002 "${output}")

vera_add_test(RuleT013
  "" "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no copyright notice found\n"
  "" 0
  --rule T013
  --root "${CMAKE_SOURCE_DIR}"
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

# Rules to test
# DUMP
# T001
# T002
# T003
# T004
# T005
# T006
# T007
# T008
# T009
# T010
# T011
# T012
# T013
# T014
# T015
# T016
# T017
# T018
# T019
