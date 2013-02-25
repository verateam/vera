<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="text"/>

  <xsl:template match="/">
    <xsl:apply-templates select="//test-parameters" />
  </xsl:template>

  <xsl:template match="//test-parameters">
    <xsl:for-each select="test-param">
      <xsl:value-of select="./@name" />
      <xsl:text>=</xsl:text>
      <xsl:value-of select="./@value" />
      <xsl:text>
</xsl:text>
    </xsl:for-each>
  </xsl:template>

</xsl:stylesheet>
