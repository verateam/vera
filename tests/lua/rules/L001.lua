-- No trailing whitespace

for file in getSourceFileNames() do
    local count = 0
    local f = assert(io.open(file, "r"))
    for line in f:lines() do
        count = count + 1
        if string.match(line, "%s+$") then
            report(file, count, "trailing whitespace")
        end
    end
    f:close()
end
