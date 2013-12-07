# no trailing space
for f in vera.getSourceFileNames():
  for lineNumber, line in enumerate(vera.getAllLines(f)):
    if line.endswith(" "):
      vera.report(f, lineNumber+1, "trailing whitespace")
