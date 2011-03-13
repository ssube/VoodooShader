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
      <link rel="stylesheet" href="voodoolog.css" media="all" />
      <body>
        <table class="sortable">
          <thead>
            <tr>
		    <th>Event #<br />(of <xsl:value-of select="count(child::Message)" />)</th>
              <th>Timestamp</th>
              <th>Type</th>
	      <th>Ticks<br />(of <xsl:value-of select="child::Message[last()]/@tick - /LogFile/@tick" />)</th>
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
              <td>
                <xsl:value-of select="@timestamp" />
              </td>
              <xsl:choose>
                <xsl:when test="@type = 'fatal'">
                  <td sorttable_customkey="0">
                    <xsl:value-of select="@type" />
                  </td>
                </xsl:when>
                <xsl:when test="@type = 'error'">
                  <td sorttable_customkey="1">
                    <xsl:value-of select="@type" />
                  </td>
                </xsl:when >
                <xsl:when test="@type = 'warning'">
                  <td sorttable_customkey="3">
                    <xsl:value-of select="@type" />
                  </td>
                </xsl:when >
                <xsl:when test="@type = 'info'">
                  <td sorttable_customkey="4">
                    <xsl:value-of select="@type" />
                  </td>
                </xsl:when >
                <xsl:when test="@type = 'debug'">
                  <td sorttable_customkey="5">
                    <xsl:value-of select="@type" />
                  </td>
                </xsl:when >
                <xsl:otherwise>
                  <td sorttable_customkey="2">
                    <xsl:value-of select="@type" />
                  </td>
                </xsl:otherwise>
              </xsl:choose>
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
