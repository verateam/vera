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
    document.getElementById("rule_source").style.display = "block";
}
function hideCode()
{
    document.getElementById("code_link").style.display = "block";
    document.getElementById("rule_source").style.display = "none";
}
]]>
</script>
      </head>
      <body>
        <xsl:for-each select="/rule">
          <h1 class="rule_id"><xsl:value-of select="./@name" /></h1>
        </xsl:for-each>
        <xsl:apply-templates select="//title" />
        <p class="description"><b>Description:</b></p>
        <xsl:apply-templates select="//description" />
        <xsl:apply-templates select="//parameters" />
        <p class="compliance">
          <b>Compliance:</b>
          <xsl:apply-templates select="//compliance" />
        </p>
        <div id="code_link" style="display:block;">
          <p class="virtual_link"><a onclick="showCode()">Show source code for this rule</a></p>
        </div>
        <div id="rule_source" style="display:none;">
          <p class="virtual_link"><a onclick="hideCode()">Hide source code</a></p>
          <pre class="example">
            <xsl:apply-templates select="//script" />
          </pre>
        </div>
        <p><a href="index.html">Rule index</a></p>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="//title" mode="head">
    <title>
      <xsl:value-of select="." />
    </title>
  </xsl:template>
  <xsl:template match="//title">
    <h1 class="rule_short"><xsl:value-of select="." /></h1>
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

  <xsl:template match="//parameters">
    <p>
      <b>Recognized parameters:</b>
    </p>
    <table class="parameters" border="1" cellpadding="5" cellspacing="0">
      <tr><th>Name</th><th>Default</th><th>Description</th></tr>
    <xsl:for-each select="param">
      <tr>
        <td>
          <code>
            <xsl:value-of select="./@name" />
          </code>
        </td>
        <td align="right">
          <code>
            <xsl:value-of select="./@default" />
          </code>
        </td>
        <td>
          <xsl:value-of select="." />
        </td>
      </tr>
    </xsl:for-each>
  </table>
  </xsl:template>

  <xsl:template match="//compliance">
    <xsl:for-each select="./standard">
      <xsl:text> </xsl:text>
      <xsl:choose>
        <xsl:when test='@name = "ISO"'>
          <abbr title="This rule directly results from the ISO standard.">
            <xsl:value-of select="@name" />
          </abbr>
        </xsl:when>
        <xsl:when test='@name = "HICPP"'>
          <abbr title="This rule is recommended by the High-Integrity C++ coding standard.">
            <xsl:value-of select="@name" />
          </abbr>
        </xsl:when>
        <xsl:when test='@name = "JSF"'>
          <abbr title="This rule is recommended by the Joint Strike Fighter C++ coding standard.">
            <xsl:value-of select="@name" />
          </abbr>
        </xsl:when>
        <xsl:when test='@name = "Boost"'>
          <abbr title="This rule is recommended by the Boost community.">
            <xsl:value-of select="@name" />
          </abbr>
        </xsl:when>
        <xsl:when test='@name = "CCS"'>
          <abbr title="This rule is recommended by the C++ Coding Standards (Sutter &amp; Alexandrescu).">
            C++ Coding Standards
          </abbr>
        </xsl:when>
        <xsl:when test='@name = "Inspirel"'>
          <abbr title="This rule is recommended by Inspirel.">
            <xsl:value-of select="@name" />
          </abbr>
        </xsl:when>
      </xsl:choose>
      <xsl:if test="position() != last()"><xsl:text>,</xsl:text></xsl:if>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="//script">
    <xsl:value-of select="." />
  </xsl:template>

</xsl:stylesheet>
