# just to show a way to use vera++
file(GLOB_RECURSE srcs ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.h)
add_test(NAME VeraStyle
  COMMAND vera
  --error
  --root "${CMAKE_SOURCE_DIR}"
  ${srcs}
)

# now the real tests

include(${CMAKE_SOURCE_DIR}/cmake/use_vera++.cmake)

####### command line iterface tests #######

vera_add_test(NoRoot
  "" "" "vera++: Cannot open profile description for profile 'default': No such file or directory\n" 1
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp"
)

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

vera_add_test(InputsDash
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp\n"
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no copyright notice found\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --inputs=-
)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/inputs
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp\n")
vera_add_test(InputsFile
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no copyright notice found\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --inputs=${CMAKE_CURRENT_BINARY_DIR}/inputs
)

vera_add_test(ShortInputs
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

vera_add_test(CheckStyleReport
  ""
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<checkstyle version=\"5.0\">
    <file name=\"${CMAKE_CURRENT_SOURCE_DIR}/test.cpp\">
        <error source=\"T013\" severity=\"info\" line=\"1\" message=\"no copyright notice found\" />
    </file>
</checkstyle>\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --checkstyle-report=-
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(ShortCheckStyleReport
  ""
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<checkstyle version=\"5.0\">
    <file name=\"${CMAKE_CURRENT_SOURCE_DIR}/test.cpp\">
        <error source=\"T013\" severity=\"info\" line=\"1\" message=\"no copyright notice found\" />
    </file>
</checkstyle>\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  -c -
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

vera_add_test(NoDuplicate
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/T009.cpp:10: comma should not be preceded by whitespace
${CMAKE_CURRENT_SOURCE_DIR}/T009.cpp:12: comma should not be preceded by whitespace
${CMAKE_CURRENT_SOURCE_DIR}/T009.cpp:13: comma should not be preceded by whitespace
${CMAKE_CURRENT_SOURCE_DIR}/T009.cpp:14: comma should not be preceded by whitespace\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --rule T009
  --no-duplicate
  ${CMAKE_CURRENT_SOURCE_DIR}/T009.cpp
)

vera_add_test(ShortNoDuplicate
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/T009.cpp:10: comma should not be preceded by whitespace
${CMAKE_CURRENT_SOURCE_DIR}/T009.cpp:12: comma should not be preceded by whitespace
${CMAKE_CURRENT_SOURCE_DIR}/T009.cpp:13: comma should not be preceded by whitespace
${CMAKE_CURRENT_SOURCE_DIR}/T009.cpp:14: comma should not be preceded by whitespace\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --rule T009
  -d
  ${CMAKE_CURRENT_SOURCE_DIR}/T009.cpp
)

vera_add_test(Parameter
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:1: line is longer than 99 characters
${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:2: line is longer than 99 characters\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --rule L004
  --parameter max-line-length=99
  ${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp
)

vera_add_test(ParameterInvalid
  "" ""
  "vera++: Invalid parameter association: max-line-length\n" 1
  --root "${CMAKE_SOURCE_DIR}"
  --rule L004
  --parameter max-line-length
  ${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp
)

vera_add_test(ShortParameter
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:1: line is longer than 99 characters
${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:2: line is longer than 99 characters\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --rule L004
  -P max-line-length=99
  ${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp
)

vera_add_test(Parameters
  ""
  "${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:1: line is longer than 99 characters
${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:2: line is longer than 99 characters\n"
  "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --rule L004
  --parameters ${CMAKE_CURRENT_SOURCE_DIR}/parameters.txt
  ${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp
)

vera_add_test(Profile
  "" "${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:1: L004: line is longer than 100 characters
${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:1: T013: no copyright notice found
${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:1: T014: no reference to the Boost Software License found
${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:2: L004: line is longer than 100 characters\n" "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --show-rule
  --profile test
  --parameter max-directory-depth=20
  ${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp
)

vera_add_test(ShortProfile
  "" "${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:1: L004: line is longer than 100 characters
${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:1: T013: no copyright notice found
${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:1: T014: no reference to the Boost Software License found
${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:2: L004: line is longer than 100 characters\n" "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --show-rule
  -p test
  --parameter max-directory-depth=20
  ${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp
)

vera_add_test(ProfileLegacy
  "" "${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:1: L004: line is longer than 100 characters
${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:1: T013: no copyright notice found
${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:2: L004: line is longer than 100 characters\n" "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --show-rule
  --profile legacy
  --parameter max-directory-depth=20
  ${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp
)

vera_add_test(Exclusions
  "" "${CMAKE_CURRENT_SOURCE_DIR}/T003.cpp:1: keyword 'case' not followed by a single space
${CMAKE_CURRENT_SOURCE_DIR}/T003.cpp:5: keyword 'explicit' not followed by a single space
${CMAKE_CURRENT_SOURCE_DIR}/T003.cpp:9: keyword 'struct' not followed by a single space\n" "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --rule T003
  --parameter max-directory-depth=20
  --exclusions ${CMAKE_CURRENT_SOURCE_DIR}/exclusions
  ${CMAKE_CURRENT_SOURCE_DIR}/T003.cpp
)

vera_add_test(ExclusionsLegacy
  "" "${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:1: L004: line is longer than 100 characters
${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:1: T013: no copyright notice found
${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp:2: L004: line is longer than 100 characters\n" "" 0
  --root "${CMAKE_SOURCE_DIR}"
  --show-rule
  --profile test
  --parameter max-directory-depth=20
  --exclusions ${CMAKE_CURRENT_SOURCE_DIR}/exclusions_legacy
  ${CMAKE_CURRENT_SOURCE_DIR}/L004.cpp
)


# just make sure the help test does not fail
add_test(NAME Help COMMAND vera --help foo bar)
add_test(NAME ShortHelp COMMAND vera -h foo bar)

# options to test
# --transform arg

####### rule tests #######

set(VERA_TEST_RULE_ROOT ${CMAKE_SOURCE_DIR})

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

vera_add_rule_test(T003 "1: keyword 'case' not followed by a single space
2: keyword 'class' not followed by a single space
3: keyword 'delete' not followed by a single space
4: keyword 'enum' not followed by a single space
5: keyword 'explicit' not followed by a single space
6: keyword 'extern' not followed by a single space
7: keyword 'goto' not followed by a single space
8: keyword 'new' not followed by a single space
9: keyword 'struct' not followed by a single space
10: keyword 'union' not followed by a single space
11: keyword 'using' not followed by a single space
13: keyword 'case' not followed by a single space
14: keyword 'class' not followed by a single space
15: keyword 'delete' not followed by a single space
16: keyword 'enum' not followed by a single space
17: keyword 'explicit' not followed by a single space
18: keyword 'extern' not followed by a single space
19: keyword 'goto' not followed by a single space
20: keyword 'new' not followed by a single space
21: keyword 'struct' not followed by a single space
22: keyword 'union' not followed by a single space
23: keyword 'using' not followed by a single space")

vera_add_rule_test(T004 "25: colon not immediately after the 'public' keyword
28: colon not immediately after the 'protected' keyword
30: colon not immediately after the 'private' keyword
39: colon not immediately after the 'default' keyword")

vera_add_rule_test(T005 "19: keyword 'break' not immediately followed by a semicolon
23: keyword 'continue' not immediately followed by a semicolon")

vera_add_rule_test(T006 "24: keyword 'throw' not immediately followed by a semicolon or a single space
25: keyword 'return' not immediately followed by a semicolon or a single space
29: keyword 'return' not immediately followed by a semicolon or a single space")

vera_add_rule_test(T007 "6: semicolon is isolated from other tokens
7: semicolon is isolated from other tokens
9: semicolon is isolated from other tokens")

vera_add_rule_test(T008 "23: keyword 'catch' not followed by a single space
25: keyword 'for' not followed by a single space
27: keyword 'if' not followed by a single space
29: keyword 'while' not followed by a single space")

vera_add_rule_test(T009 "10: comma should not be preceded by whitespace
12: comma should not be preceded by whitespace
12: comma should not be preceded by whitespace
13: comma should not be preceded by whitespace
13: comma should not be preceded by whitespace
14: comma should not be preceded by whitespace")

vera_add_rule_test(T010 "5: identifier should not be composed of only 'l' and 'O'
6: identifier should not be composed of only 'l' and 'O'")

vera_add_rule_test(T011 "22: closing curly bracket not in the same line or column
27: closing curly bracket not in the same line or column
30: closing curly bracket not in the same line or column
39: closing curly bracket not in the same line or column")

vera_add_rule_test(T012 "1: negation operator used in its short form")

vera_add_test(RuleT013
  "" "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no copyright notice found\n"
  "" 0
  --rule T013
  --root "${CMAKE_SOURCE_DIR}"
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/T013.cpp
)

vera_add_test(RuleT014
  "" "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp:1: no reference to the Boost Software License found\n"
  "" 0
  --rule T014
  --root "${CMAKE_SOURCE_DIR}"
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/T013.cpp
)

vera_add_rule_test(T015 "8: URL points to non-existing file
9: URL points to non-existing file")

vera_add_rule_test(T016 "1: min/max potential macro substitution problem")

vera_add_test(RuleT017
  "" "${CMAKE_CURRENT_SOURCE_DIR}/T017.h:1: unnamed namespace not allowed in header file\n"
  "" 0
  --rule T017
  --root "${CMAKE_SOURCE_DIR}"
  ${CMAKE_CURRENT_SOURCE_DIR}/T017.h
  ${CMAKE_CURRENT_SOURCE_DIR}/T017.cpp
)

vera_add_test(RuleT018
  "" "${CMAKE_CURRENT_SOURCE_DIR}/T018.h:1: using namespace not allowed in header file\n"
  "" 0
  --rule T018
  --root "${CMAKE_SOURCE_DIR}"
  ${CMAKE_CURRENT_SOURCE_DIR}/T018.h
  ${CMAKE_CURRENT_SOURCE_DIR}/T018.cpp
)

vera_add_rule_test(T019 "1: full block {} expected in the control structure
5: full block {} expected in the control structure
17: full block {} expected in the control structure
33: full block {} expected in the control structure
41: full block {} expected in the control structure
42: full block {} expected in the control structure
52: full block {} expected in the control structure
63: full block {} expected in the control structure
64: full block {} expected in the control structure")

vera_add_test(RuleDUMP
  "" "Tokens in file ${CMAKE_CURRENT_SOURCE_DIR}/T017.cpp:
1/0\tnamespace\tnamespace
1/9\tnewline\t

2/0\tleftbrace\t{
2/1\tnewline\t

3/0\tcppcomment\t// some stuff here

4/0\tmsext_stdcall\t__stdcall
4/9\tnewline\t

5/0\trightbrace\t}
5/1\tnewline\t

6/0\teof\t

"
  "" 0
  --rule DUMP
  --root "${CMAKE_SOURCE_DIR}"
  ${CMAKE_CURRENT_SOURCE_DIR}/T017.cpp
)

vera_add_test(ErrorReport
  "" "" "vera++: can't read \"auie\": no such variable
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
    (file \"${CMAKE_CURRENT_SOURCE_DIR}/errorReport/scripts/rules/Broken.tcl\" line 9)\n"
  1
  --rule Broken
  --root "${CMAKE_CURRENT_SOURCE_DIR}/errorReport"
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(InvalidToken
  "" "${CMAKE_CURRENT_SOURCE_DIR}/invalidToken.cpp:2: L001: trailing whitespace
${CMAKE_CURRENT_SOURCE_DIR}/invalidToken.cpp:3: vera++ internal: illegal token in column 12, giving up (hint: fix the file or remove it from the working set)\n"
  "" 0
  --rule L001 --show-rule
  --root "${CMAKE_SOURCE_DIR}"
  ${CMAKE_CURRENT_SOURCE_DIR}/invalidToken.cpp
)

vera_add_test(InvalidCastReport
  "" "" "vera++: Can't cast '' to int
    while executing
\"report $fileName $lineNumber $msg\"
    (file \"${CMAKE_CURRENT_SOURCE_DIR}/errorReport/scripts/rules/cast.tcl\" line 8)\n"
  1
  --rule cast
  --root "${CMAKE_CURRENT_SOURCE_DIR}/errorReport"
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

vera_add_test(NegativeLineNumberReport
  "" "" "vera++: Line number out of range: -4
    while executing
\"report \"fileName\" -4 \"foo\"\"
    (file \"${CMAKE_CURRENT_SOURCE_DIR}/errorReport/scripts/rules/negativeLine.tcl\" line 3)\n"
  1
  --rule negativeLine
  --root "${CMAKE_CURRENT_SOURCE_DIR}/errorReport"
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

if(VERA_PYTHON)
  add_subdirectory(python)
endif()

if(VERA_LUA)
  add_subdirectory(lua)
endif()
