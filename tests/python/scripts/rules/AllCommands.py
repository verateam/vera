# test that Python has access to all the commands
for f in vera.getSourceFileNames():
  print "FileName: " + f
  print "getLineCount: " + str(vera.getLineCount(f))
  print "getLine: " + vera.getLine(f, 1)
  print "getAllLines..."
  for lineNumber, line in enumerate(vera.getAllLines(f)):
    print "lineNumber, line: " + str(lineNumber) + ", " + line

  for t in vera.getTokens(f, 1, 0, -1, -1, []):
    print "Token: l,c,t,n,v: " + str(t.line) + ", " + str(t.column) + ", " + t.type + ", " + t.name + ", " + t.value

  p = vera.getParameter("python_test_param_exists", "default")
  print "getParameter: " + p
  d = vera.getParameter("python_test_param_does_not_exist", "default")
  print "getParameter (use default): " + d
  vera.report(f, 1, "reporting line 1")
