<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="text"/>

  <xsl:template match="/">
    <xsl:text>&lt;tr&gt;</xsl:text>
    <xsl:text>&lt;td&gt;</xsl:text>
    <xsl:apply-templates select="//rule|//transformation" />
    <xsl:text>&lt;/td&gt;</xsl:text>
    <xsl:text>&lt;td&gt;</xsl:text>
    <xsl:apply-templates select="//title" />
    <xsl:text>&lt;/td&gt;</xsl:text>
    <xsl:text>&lt;/tr&gt;</xsl:text>
  </xsl:template>

  <xsl:template match="//rule|//transformation">
    <xsl:text>&lt;a href="</xsl:text>
    <xsl:value-of select="@name" />
    <xsl:text>.html"></xsl:text>
    <xsl:value-of select="@name" />
    <xsl:text>&lt;/a&gt; </xsl:text>
  </xsl:template>

  <xsl:template match="//title">
    <xsl:value-of select="." />
  </xsl:template>

</xsl:stylesheet>
