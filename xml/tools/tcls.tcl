set ruleXmls [glob ../rules/*.xml]
foreach x $ruleXmls {
    set ruleName [file rootname [file tail $x]]
    set scriptName ../../scripts/rules/${ruleName}.tcl
    puts "Creating $scriptName"
    exec xsltproc xslt/xml2tcl.xsl $x > $scriptName
}

set transformXmls [glob ../transformations/*.xml]
foreach x $transformXmls {
    set transformationName [file rootname [file tail $x]]
    set scriptName ../../scripts/transformations/${transformationName}.tcl
    puts "Creating $scriptName"
    exec xsltproc xslt/xml2tcl.xsl $x > $scriptName
}
