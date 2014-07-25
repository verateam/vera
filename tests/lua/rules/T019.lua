-- control structures should have complete curly-braced block of code

filter = {"for", "if", "while", "do", "else", "leftparen", "rightparen", "leftbrace", "rightbrace", "semicolon"}
START, CONTROL, EXPECTED_BLOCK, BLOCK = 0, 1, 2, 3

for fileName in getSourceFileNames() do
  state = START
  prev = nil
  for t in getTokens(fileName, 1, 0, -1, -1, filter) do
    if state == CONTROL then
      if t.type == "leftparen" then
        parenCount = parenCount + 1
      elseif t.type == "rightparen" then
        parenCount = parenCount - 1
        if parenCount == 0 then
          state = EXPECTED_BLOCK
        end
      end
    elseif state == EXPECTED_BLOCK then
      if prev == "else" and t.type == "if" then
        -- do nothing
      elseif t.type ~= "leftbrace" then
        report(fileName, t.line, "full block {} expected in the control structure")
      end
      state = BLOCK
    end

    if t.type == "for" or t.type == "if" then
      parenCount = 0
      state = CONTROL
    elseif t.type == "do" or t.type == "else" then
      state = EXPECTED_BLOCK
    elseif t.type == "while" and prev ~= "rightbrace" then
      parenCount = 0
      state = CONTROL
    end
    prev = t.type
  end
end
