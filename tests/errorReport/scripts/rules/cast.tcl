#!/usr/bin/tclsh
# segmentation fault error

set fileName "fileName"
set lineNumber ""; #let's use an empty string instead of integer for line number
set msg "an example report message"

report $fileName $lineNumber $msg
