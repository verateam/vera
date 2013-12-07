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
