set(VERA_TEST_RULE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

vera_add_rule_test(L001.py "4: trailing whitespace
6: trailing whitespace")

vera_add_rule_test(T019.py "1: full block {} expected in the control structure
5: full block {} expected in the control structure
17: full block {} expected in the control structure
33: full block {} expected in the control structure
41: full block {} expected in the control structure
42: full block {} expected in the control structure
52: full block {} expected in the control structure
61: full block {} expected in the control structure
62: full block {} expected in the control structure")

vera_add_test(AllCommandMappingsForPython 
  ""
  "FileName: ${CMAKE_CURRENT_SOURCE_DIR}/AllCommands.py.cpp
getLineCount: 2
getLine: /* A comment on the first line. */
getAllLines...
lineNumber, line: 0, /* A comment on the first line. */
lineNumber, line: 1, #define MACRO(x) (x+1)
Token: l,c,t,n,v: 1, 0, ccomment, ccomment, /* A comment on the first line. */
Token: l,c,t,n,v: 1, 34, newline, newline, 

Token: l,c,t,n,v: 2, 0, pp_define, pp_define, #define
Token: l,c,t,n,v: 2, 7, space, space,  
Token: l,c,t,n,v: 2, 8, identifier, identifier, MACRO
Token: l,c,t,n,v: 2, 13, leftparen, leftparen, (
Token: l,c,t,n,v: 2, 14, identifier, identifier, x
Token: l,c,t,n,v: 2, 15, rightparen, rightparen, )
Token: l,c,t,n,v: 2, 16, space, space,  
Token: l,c,t,n,v: 2, 17, leftparen, leftparen, (
Token: l,c,t,n,v: 2, 18, identifier, identifier, x
Token: l,c,t,n,v: 2, 19, plus, plus, +
Token: l,c,t,n,v: 2, 20, intlit, intlit, 1
Token: l,c,t,n,v: 2, 21, rightparen, rightparen, )
Token: l,c,t,n,v: 2, 22, newline, newline, 

Token: l,c,t,n,v: 3, 0, eof, eof, 
getParameter: waspassedin
getParameter (use default): default
${CMAKE_CURRENT_SOURCE_DIR}/AllCommands.py.cpp:1: reporting line 1\n"
  "" 0
  --root "${CMAKE_CURRENT_SOURCE_DIR}"
  --rule AllCommands
  --parameter python_test_param_exists=waspassedin
  ${CMAKE_CURRENT_SOURCE_DIR}/AllCommands.py.cpp
)
