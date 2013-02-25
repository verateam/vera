set line [gets stdin]
while {![eof stdin]} {
    if {[string index $line 0] == "@"} {
        if [info exists file] {
            close $file
        }
        set file [open [lindex [split $line] 1] "w"]
    } else {
        puts $file $line
    }

    set line [gets stdin]
}

close $file
