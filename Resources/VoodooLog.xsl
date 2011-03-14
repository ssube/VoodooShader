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
        <div class="section">
          <div class="section_title">Log Information:</div>
          <xsl:variable name="runtime" select="child::Message[last()]/@tick - /LogFile/@tick" />
          Log contains <xsl:value-of select="count(child::Message)" /> entries over a run time of
          <xsl:choose>
            <xsl:when test="$runtime &gt;= 60000">
              <xsl:variable name="minutes" select="floor($runtime div 60000)" />
              <xsl:value-of select="$minutes" /> minutes and <xsl:value-of select="round(($runtime mod 60000) div 1000)" /> seconds 
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$runtime div 1000" /> seconds 
            </xsl:otherwise>
          </xsl:choose>
          (<xsl:value-of select="$runtime" /> ticks).<br />
          Log file was started on <xsl:value-of select="/LogFile/@date" /> at <xsl:value-of select="/LogFile/@time" />.
        </div>
        <div class="section">
          <div class="section_title">Loaded Modules:</div>
          <table class="modules">
            <thead>
              <tr>
                <th>Name</th>
                <th>Major</th>
                <th>Minor</th>
                <th>Patch</th>
                <th>Revision</th>
              </tr>
            </thead>
            <xsl:for-each select="Module">
              <tr>
                <td>
                  <xsl:value-of select="@name" />
                </td>
                <td>
                  <xsl:value-of select="@major" />
                </td>
                <td>
                  <xsl:value-of select="@minor" />
                </td>
                <td>
                  <xsl:value-of select="@patch" />
                </td>
                <td>
                  <xsl:value-of select="@rev" />
                </td>
              </tr>
            </xsl:for-each>
          </table>
        </div>
        <div class="section">
          <div class="section_title">Log Events:</div>
          <table class="sortable" width="100%" height="100%">
            <thead>
              <tr>
                <th>Event #</th>
                <th>Severity</th>
                <th>Ticks</th>
                <th>Step</th>
                <th>Source</th>
                <th>Body</th>
              </tr>
            </thead>
            <xsl:for-each select="Message">
              <xsl:text disable-output-escaping="yes">&lt;tr class="level_</xsl:text>
              <!-- Round to the nearest 0x10 for simpler stylesheet -->
              <xsl:choose>
                <xsl:when test="not(@level)">0</xsl:when>
                <xsl:when test="@level &gt; 250">255</xsl:when>
                <xsl:when test="@level &lt; 10">0</xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="@level - (@level mod 16)" />
                </xsl:otherwise>
              </xsl:choose>
              <xsl:text disable-output-escaping="yes">"&gt;</xsl:text>
              <td>
                <xsl:value-of select="position()" />
              </td>
              <td sorttable_customkey="{@level}">
                <xsl:value-of select="@level" />
              </td>
              <td>
                <xsl:value-of select="@tick - /LogFile/@tick" />
              </td>
              <td>
                <xsl:value-of select="@tick - current()/preceding-sibling::Message[position()=1]/@tick" />
              </td>
              <td>
                <xsl:value-of select="@module" />
              </td>
              <td>
                <xsl:value-of select="." />
              </td>
              <xsl:text disable-output-escaping="yes">&lt;/tr&gt;</xsl:text>
            </xsl:for-each>
          </table>
        </div>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>
