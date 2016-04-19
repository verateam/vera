#!/usr/bin/tclsh
# Types have an initial uppercase letter.
# Objects have an initial lowercase letter.
#
#   Looking for variable declarations:
#       TypeName    objectName              ie Identifier Identifier
#
#   Take into account
#       #if defined Value                   ignore pre-processor macro line
#       #error AnyWord AnotherWord          ignore pre-processor macro line
#       typedef Type Type;                  ignore typedef upto ';'
#       template<Type TemplateType>         drop template or template< Stuff upto >
#       long long objectName;               built in type behave like an extended identifer
#       Type  NameSpace::Class::object      fully qualified names are ignored.
#                                           these probably belong to a package not owned by
#                                           this project anyway.
#

# States:
#   Ignore lines with pre-processor macros
#
#
#
#
#          |--------------------------------------------------------------
#          |               |                      |                      |                           Not Greater(>)
#          |          #if or #error            typedef               templatea     Less(<)              -------
#          |          or #define                  |    -----             |    ----------------          |     |
#          |              \/                     \/    |  Not ;         \/    |              \/        \/     |
#          |        ##############         ##############  |      ##############             ##############   |
#          |        # PreProc    #         # Typedef    #  |      # Template   # <|          # Template<  #   |
#          |        ##############         ##############  |      ##############  | Space    ##############   |
#          |               |                     |    /\   |       |          |   |             |        |    |
#          |             New Line                ;     -----  (Not Less(<) && -----        Greater(>)    -----
#          |               |                     |             Not Space( ))
#   ##############         |                     |                 |                            |
#   # start      #<------------------------------<-----------------<-----------------------------
#   ##############
#          |
#          |
#          |
#          |
#      Identifier              ---->-<-----------------<------------------------------      ------
#   or BuiltIn Type            |    |                  |                             |      |    |
#          |         BuiltIn Type   |              Greater(>)                    Identifier |   Space
#          |         or space  |    |                  |                             |      |    |
#          |       ##############   |                  ##############           ##############   |
#          -------># Found1     #<---                  # Found1<    #<---       # Found1::   #<---
#                  ##############                      ##############   |       ##############
#                       |      |                         /\        |    |              /\
#                       |      |                      Less(<)   Not Greater(>)         ::
#                       |      |                         |         ------              |
#                       |      ---------------------------------------------------------
#                       |
#    ##############     |
#    # Hit Check  #     |
#    # Identifer  #     |
#    # Reset to   #<-----
#    # Start      #   Identifier
#    ##############

set builtInTypes {
    bool
    signed
    unsigned
    long
    int
    short
    char
    float
    double
}

proc isBuiltInType {s} {
    global builtInTypes
    return [expr [lsearch $builtInTypes $s] != -1]
}

set state "start"
set pp_line -1

foreach f [getSourceFileNames] {
    foreach t [getTokens $f 1 0 -1 -1 {}] {
        set identifier [lindex $t 0]
        set tokenName [lindex $t 3]
        set lineNumber [lindex $t 1]
        if {$state == "Template" && ($tokenName != "less" && $tokenName != "space")} {
            set state "start"
        }

        if {$tokenName == "pp_if" || $tokenName == "pp_error" || $tokenName == "pp_define"} {
            set pp_line $lineNumber
            #puts "$identifier: => PP Start"
        } elseif {$lineNumber == $pp_line} {
            # Ignore PP line tokens
            #puts "$tokenName -> $identifier: => PP Cont"
        } elseif {$state == "start" && $tokenName == "typedef"} {
            set state "Typedef"
            #puts "$tokenName -> $identifier: => Typedef Start"
        } elseif {$state == "Typedef"} {
            if {$tokenName == "semicolon"} {
                set state "start"
                #puts "$tokenName -> $identifier: => Typedef END"
            } else {
                #puts "$tokenName -> $identifier: => Typedef Continue"
            }
        } elseif {$state == "start" && $tokenName == "template"} {
            set state "Template"
            #puts "$tokenName -> $identifier: => Template Start"
        } elseif {$state == "Template" && $tokenName == "less"} {
            set state "Template<"
            #puts "$tokenName -> $identifier: => Template Open"
        } elseif {$state == "Template<" && $tokenName == "greater"} {
            set state "start"
            #puts "$tokenName -> $identifier: => Template Close (start)"
        } elseif {$state == "start" && $tokenName == "identifier"} {
            set state "Found1"
            #puts "$tokenName -> $identifier: => start -> Found1"
        } elseif {$state == "start" && [isBuiltInType $tokenName]} {
            set state "Found1"
            #puts "$tokenName -> $identifier: => start -> Found1  Built in"
        } elseif {$state == "Found1" && $tokenName == "space"} {
            #ignore
            #puts "$tokenName -> $identifier: => Found1 -> Found1 space"
        } elseif {$state == "Found1" && $tokenName == "colon_colon"} {
            set state "Found1::"
            #puts "$tokenName -> $identifier: => Found1 -> Found1 ::"
        } elseif {$state == "Found1::" && $tokenName == "identifier"} {
            set state "Found1"
            #puts "$tokenName -> $identifier: => Found1:: -> Found1
        } elseif {$state == "Found1::" && $tokenName == "space"} {
            #ignore
            #puts "$tokenName -> $identifier: => Found1:: -> Found1:: space
        } elseif {$state == "Found1" && [isBuiltInType $tokenName]} {
            #ignore 
            #puts "$tokenName -> $identifier: => Found1 -> Found1 Built in"
        } elseif {$state == "Found1" && $tokenName == "less"} {
            set state "Found1<"
            #puts "$tokenName -> $identifier: => Found1 -> Found1< Template Param"
        } elseif {$state == "Found1<" && $tokenName != "great"} {
            #ignore
            #puts "$tokenName -> $identifier: => Found1 -> Found1< Template Param Cont"
        } elseif {$state == "Found1<" && $tokenName == "great"} {
            set state "Found1"
            #puts "$tokenName -> $identifier: => Found1< -> Found1"
        } elseif {$state == "Found1" && $tokenName == "identifier"} {
            #puts "$tokenName -> $identifier: => Found2 -> HIT Checking"
            set state "start"
            # Hit
            set objectName [lindex $t 0]
            set columnNumber [lindex $t 2]
            set nextToken [getTokens $f $lineNumber [expr $columnNumber + [string length $objectName]] [expr $lineNumber + 1] -1 {}]
            set ignore 0
            foreach next $nextToken {
                set nextTokenName [lindex $next 3]
                if {$nextTokenName == "space"} {
                    # Continue to next token
                } elseif {$nextTokenName == "colon_colon"} {
                    # Fully qualified identifier so ignore
                    set ignore 1
                    break
                } else {
                    # Any other token mean we should look at it
                    break
                }
            }
            set identifier [lindex $t 0]
            set identifierFirst [string index $identifier 0]
            if {$ignore == 0 && [expr ! [string is lower $identifierFirst]]} {
                report $f $lineNumber "Objects >$identifier< (variables/functions) should have an initial lower case letter"
            }
        } else {
            #puts "$tokenName -> $identifier: => Reset to start"
            set state "start"
        }
    }
}
