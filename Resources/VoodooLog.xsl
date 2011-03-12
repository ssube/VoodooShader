<?xml version="1.0" encoding="utf-8"?>
<!-- Voodoo Log XSL Style Sheet -->
<!-- Copyright 2010 by Sean "peachykeen" Sube-->
<!-- All Rights Reserved. Please contact peachykeen@voodooshader.com for usage or questions. -->
<xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:msxsl="urn:schemas-microsoft-com:xslt" 
    exclude-result-prefixes="msxsl"
>
  <xsl:output method="html" indent="yes"/>

  <xsl:template match="LogFile">
    <html>
      <title>Voodoo Shader Framework Log</title>
      <script src="sorttable.js"></script>
      <style>
        table tr
        {
        background: #DDDDDD;
        }
        .debug
        {
        background: #CCCCDD;
        }
        .info
        {
        background: #DCDCDC;
        }
        .warning
        {
        background: #FFFF33;
        }
        .error
        {
        background: #FF0000;
        color: #CCCCCC;
        }
        .fatal
        {
        background: #880000;
        color: #FFFFFF;
        }
      </style>
      <body>
        <table class="sortable">
          <thead>
            <tr>
              <th># (<xsl:value-of select="count(child::Message)" />)</th>
              <th>Type</th>
              <th>Timestamp</th>
              <th>Relative</th>
              <th>Step</th>
              <th>Source</th>
              <th>Body</th>
            </tr>
          </thead>
          <xsl:for-each select="Message">
            <tr class="{@type}">
              <td>
                <xsl:value-of select="position()" />
              </td>
              <xsl:if test="@type = 'fatal'">
                <xsl:variable name="level" select="0" />
              </xsl:if >
              <xsl:if test="@type = 'error'">
                <xsl:variable name="level" select="1" />
              </xsl:if >
              <xsl:if test="@type = 'warning'">
                <xsl:variable name="level" select="2" />
              </xsl:if >
              <xsl:if test="@type = 'info'">
                <xsl:variable name="level" select="3" />
              </xsl:if >
              <xsl:if test="@type = 'debug'">
                <xsl:variable name="level" select="4" />
              </xsl:if >
              <td sorttable_customkey="$level">
                <xsl:value-of select="@type" />
              </td>
              <td>
                <xsl:value-of select="@timestamp" />
              </td>
              <td>
                <xsl:value-of select="@tick - /LogFile/@tick" />
              </td>
              <td>
                <xsl:value-of select="@tick - current()/preceding-sibling::*[position()=1]/@tick" />
              </td>
              <td>
                <xsl:value-of select="@module" />
              </td>
              <td>
                <xsl:value-of select="." />
              </td>
            </tr>
          </xsl:for-each>
        </table>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>
