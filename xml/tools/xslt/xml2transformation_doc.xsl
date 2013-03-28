<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="xml"
    doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN"
    doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"
    encoding="ISO-8859-1"
    omit-xml-declaration="yes" />

  <xsl:template match="/">
    <html>
      <head>
        <link rel='stylesheet' type='text/css' href='style.css' />
        <xsl:apply-templates select="//title" mode="head" />
<script type="text/javascript">
<![CDATA[
function showCode()
{
    document.getElementById("code_link").style.display = "none";
    document.getElementById("transformation_source").style.display = "block";
}
function hideCode()
{
    document.getElementById("code_link").style.display = "block";
    document.getElementById("transformation_source").style.display = "none";
}
]]>
</script>
      </head>
      <body>
        <xsl:for-each select="/transformation">
          <h1 class="transformation_id"><xsl:value-of select="./@name" /></h1>
        </xsl:for-each>
        <xsl:apply-templates select="//title" />
        <p class="description"><b>Description:</b></p>
        <xsl:apply-templates select="//description" />
        <div id="code_link" style="display:block;">
          <p class="virtual_link"><a onclick="showCode()">Show source code for this transformation</a></p>
        </div>
        <div id="transformation_source" style="display:none;">
          <p class="virtual_link"><a onclick="hideCode()">Hide source code</a></p>
          <pre class="example">
            <xsl:apply-templates select="//script" />
          </pre>
        </div>
        <p><a href="index.html">Transformation index</a></p>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="//title" mode="head">
    <title>
      <xsl:value-of select="." />
    </title>
  </xsl:template>
  <xsl:template match="//title">
    <h1 class="transformation_short"><xsl:value-of select="." /></h1>
  </xsl:template>

  <!--   The <description> node can contain only <p>, <example> and <ul> nodes. -->
  <xsl:template match="//description">
    <xsl:apply-templates select="//p|//example|//ul" />
  </xsl:template>

  <!-- The <p> node can contain free text and other nodes (<emph> and <code>), -->
  <!-- which require some further transformation and cannot be just copied.    -->
  <xsl:template match="//p">
    <p>
      <xsl:apply-templates />
    </p>
  </xsl:template>

  <xsl:template match="//emph">
    <i>
      <xsl:value-of select="." />
    </i>
  </xsl:template>

  <xsl:template match="//code">
    <code class="inline-code">
      <xsl:value-of select="." />
    </code>
  </xsl:template>

  <xsl:template match="//example">
    <pre class="example">
      <xsl:value-of select="." />
    </pre>
  </xsl:template>

  <xsl:template match="//ul">
    <ul>
      <xsl:for-each select="li">
        <li>
          <xsl:apply-templates />        
        </li>
      </xsl:for-each>
    </ul>
  </xsl:template>

  <xsl:template match="//script">
    <xsl:value-of select="." />
  </xsl:template>

</xsl:stylesheet>
