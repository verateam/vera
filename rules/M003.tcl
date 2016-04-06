#!/usr/bin/tclsh
# using namespace XXX is not allowed
#   Unless you are in the source file X.cpp and the header file X.h contains the namespace XXX.

proc getHeaderFile {fileName} {
    return [join [list [file rootname $fileName] ".h"] ""]
}
proc getDefinedNamespaces {fileName} {
    set headerFile [getHeaderFile $fileName]
    if {[expr ! [file exists $headerFile]]} {
        return {}
    }
    set  namespaceList {}
    set  currentNamespace {}
    set  depth 0
    set  depthHistory {}
    set  state "start"
    foreach token [getTokens $headerFile 1 0 -1 -1 {namespace identifier leftbrace rightbrace assign semicolon}] {
        set tokenName [lindex $token 3]
        set tokenValue [lindex $token 0]
        if {$state == "start" && $tokenName == "namespace"} {
            set state "namespace"
        } elseif {$state == "namespace" && $tokenName == "identifier"} {
            set state "namespace-identifier"
            set saveNamespaceName $tokenValue
        } elseif {$state == "namespace-identifier" && $tokenName == "assign"} {
            set state "namespace-assign"
        } elseif {$state == "namespace-assign" && $tokenName != "semicolon"} {
            # ignore
        } elseif {$state == "namespace-assign" && $tokenName == "semicolon"} {
            set state "start"
        } elseif {$state == "namespace-identifier" && $tokenName == "leftbrace"} {
            set state "start"
            lappend currentNamespace $saveNamespaceName
            lappend depthHistory $depth
            set $depth 0
            lappend namespaceList $currentNamespace
        } elseif {$state == "start" && $tokenName == "leftbrace"} {
            set depth [expr $depth + 1]
        } elseif {$state == "start" && $tokenName == "rightbrace"} {
            if {$depth == 0} {
                set depth [lindex $depthHistory end]
                set depthHistory [lreplace $depthHistory end end]
                set currentNamespace [lreplace $currentNamespace end end]
            } else {
                set depth [expr $depth - 1]
            }
        }
    }
    return $namespaceList;
}

proc validateUsing {file mark namespace definedNamespaces} {
    set XX [lsearch $definedNamespaces $namespace]
    if {[lsearch $definedNamespaces $namespace] == -1} {
        set lineNumber [lindex $mark 1]
        report $file $lineNumber "Namespace >$namespace< is not valid for using clause"
    }
}

foreach fileName [getSourceFileNames] {
    set extension [file extension $fileName]
    if {[lsearch {.cpp .cc} $extension] != -1} {
        set definedNamespaces [getDefinedNamespaces $fileName]
        set state "start"
        foreach token [getTokens $fileName 1 0 -1 -1 {using namespace identifier colon_colon semicolon}] {
            set type [lindex $token 3]
            set value [lindex $token 0]

            if {$state == "start" && $type == "using"} {
                set mark $token
                set namespace {}
                set state "using"
            } elseif {$state == "using" && $type == "namespace"} {
                set state "namespace"
            } elseif {$state == "namespace" && $type == "identifier"} {
                set state "identifier"
                lappend namespace $value
            } elseif {$state == "identifier" && $type == "colon_colon"} {
                set state "colon_colon"
            } elseif {$state == "colon_colon" && $type == "identifier"} {
                set state "identifier"
                lappend namespace $value
            } elseif {$state == "identifier" && $type == "semicolon"} {
                validateUsing $fileName $token $namespace $definedNamespaces
                set state "start"
            } else {
            }
        }
    }
}
