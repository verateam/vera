set ruleXmls [glob ../rules/*.xml]
set indexFile ../../doc/rules/index.html
exec cp rule_index.html.head $indexFile
foreach x $ruleXmls {
    set ruleName [file rootname [file tail $x]]
    set docName "../../doc/rules/${ruleName}.html"
    puts "Creating $docName"
    exec xsltproc xslt/xml2rule_doc.xsl $x > $docName
    exec xsltproc xslt/xml2indexentry.xsl $x >> $indexFile
}
exec cat rule_index.html.tail >> $indexFile

set transformXmls [glob ../transformations/*.xml]
set indexFile ../../doc/transformations/index.html
exec cp transformation_index.html.head $indexFile
foreach x $transformXmls {
    set transformationName [file rootname [file tail $x]]
    set docName "../../doc/transformations/${transformationName}.html"
    puts "Creating $docName"
    exec xsltproc xslt/xml2transformation_doc.xsl $x > $docName
    exec xsltproc xslt/xml2indexentry.xsl $x >> $indexFile
}
exec cat transformation_index.html.tail >> $indexFile
