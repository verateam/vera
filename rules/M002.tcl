#!/usr/bin/tclsh
# Check that Type identifiers have an initial uppercase letter.

# namespace TypeName
# class TypeName
# struct TypeName
# enum TypeName {  EnumTypeName1, EnumTypeName2 }
#
# Will ignore `using clauses`
#   using <Stuff> ;
#
# Thus when "using" stuff from other namespace we don't check (as they don't belong to us)
# Also it allows you to convert any external types to local names.
#
#   #include <sys/socket.h>
#   #include <sys/types.h>
#   // STUFF
#           sockaddr_in serv_addr;
#           // STUFF
#           if (::connect(socket, reinterpret_cast<struct sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0)
#                                                   ^^^^^^^^^^^^    this fails.
# But you can write it like this:
#
#           using SockAddr = struct sockaddr;   // Allowed because >using <stuff>;< is ignored.
#           if (::connect(socket, reinterpret_cast<SockAddr*>(&serv_addr), sizeof(serv_addr)) < 0)
#


set state "start"
foreach f [getSourceFileNames] {
    foreach t [getTokens $f 1 0 -1 -1 {}] {
        set tokenName [lindex $t 3]
        set checkTypeName 0
        if {$tokenName == "space"} {
            #ignore space
        } else {
            if {$state == "start" && $tokenName == "namespace"} {
                set state "CheckIdnetifier"
            } elseif {$state == "start" && $tokenName == "class"} {
                set state "CheckIdnetifier"
            } elseif {$state == "start" && $tokenName == "struct"} {
                set state "CheckIdnetifier"
            } elseif {$state == "start" && $tokenName == "using"} {
                set state "using"
            } elseif {$state == "using" && $tokenName == "semicolon"} {
                set state "start"
            } elseif {$state == "using"} {
                #ignore
            } elseif {$state == "start" && $tokenName == "enum"} {
                set state "enum"
            } elseif {$state == "enum" && $tokenName == "identifier"} {
                set checkTypeName 1
            } elseif {$state == "enum" && $tokenName == "leftbrace"} {
                set state "enumOpen"
            } elseif {$state == "enumOpen" && $tokenName == "identifier"} {
                set checkTypeName 1
            } elseif {$state == "enumOpen" && $tokenName == "comma"} {
                # ignore
            } elseif {$state == "enumOpen" && $tokenName == "rightbrace"} {
                set state "start"
            } elseif {$state == "CheckIdnetifier" && $tokenName == "identifier"} {
                set checkTypeName 1
                set state "start"
            } else {
                set state "start"
            }
        }
        if {$checkTypeName} {
            set identifier [lindex $t 0]
            set firstLetter [string index $identifier 0]
            if {[expr ! [string is upper $firstLetter]]} {
                set lineNumber [lindex $t 1]
                report $f $lineNumber "Objects >$identifier< (Type) should have an initial upper case letter"
            }
        }
    }
}
