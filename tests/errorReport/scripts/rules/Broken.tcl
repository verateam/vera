proc Foo {} {
   return [lindex $auie]
}

proc Bar {} {
   return [Foo]
}

puts [Bar]
