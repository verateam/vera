cd temp

set rulesXmls [lsort [glob ../../rules/*.xml]]

foreach x $rulesXmls {
    set ruleName [file rootname [file tail $x]]
    puts -nonewline "Testing ${ruleName}... "

    exec xsltproc ../xslt/xml2test_source.xsl $x | tclsh ../xslt/fileSplit.tcl
    exec xsltproc ../xslt/xml2test_result.xsl $x > result.txt
    exec xsltproc ../xslt/xml2params.xsl $x > params.txt
    eval [concat exec ../../../vera++ -rule $ruleName -paramfile params.txt [glob -nocomplain *.cpp] [glob -nocomplain *.h] |& diff result.txt - ]

    eval [concat exec rm [glob *]]

    puts "OK"
}

set transformXmls [lsort [glob ../../transformations/*.xml]]
foreach x $transformXmls {
    set transformationName [file rootname [file tail $x]]
    puts -nonewline "Testing ${transformationName}... "

    exec xsltproc ../xslt/xml2test_source.xsl $x | tclsh ../xslt/fileSplit.tcl
    set outFileName [exec xsltproc ../xslt/xml2transform_outfilename.xsl $x]
    eval [concat exec ../../../vera++ -transform $transformationName test.cpp]
    eval [concat exec diff $outFileName test_transformed]

    eval [concat exec rm [glob *]]

    puts "OK"
}
