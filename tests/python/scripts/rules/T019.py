#!/usr/bin/python
# control structures should have complete curly-braced block of code

tokenTypes = ["for", "if", "while", "do", "else", "leftparen", "rightparen", "leftbrace", "rightbrace", "semicolon"]
START, CONTROL, EXPECTED_BLOCK, BLOCK = range(4)

for fileName in vera.getSourceFileNames():
  state = START
  prev = None
  for t in vera.getTokens(fileName, 1, 0, -1, -1, tokenTypes):
    if state == CONTROL:
      if t.type == "leftparen":
        parenCount += 1
      elif t.type == "rightparen":
        parenCount -= 1
        if parenCount == 0:
          state = EXPECTED_BLOCK
    elif state == EXPECTED_BLOCK:
      if prev == "else" and t.type == "if":
        pass
      elif t.type != "leftbrace":
        vera.report(fileName, t.line, "full block {} expected in the control structure")
      state = BLOCK

    if t.type == "for" or t.type == "if":
      parenCount = 0
      state = CONTROL
    elif t.type == "do" or t.type == "else":
      state = EXPECTED_BLOCK
    elif t.type == "while" and prev != "rightbrace":
      parenCount = 0
      state = CONTROL
    prev = t.type
