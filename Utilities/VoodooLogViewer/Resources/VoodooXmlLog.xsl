<?xml version="1.0" encoding="utf-8"?>
<!-- 
   - This file is part of the Voodoo Shader Framework.
   -
   - Copyright (c) 2010-2011 by Sean Sube
   -
   - The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
   - General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
   - any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
   - even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
   - License for more details.
   -
   - You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
   - the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
   -
   - Support and more information may be found at
   -   http://www.voodooshader.com
   - or by contacting the lead developer at
   -   peachykeen@voodooshader.com 
   -->
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:msxsl="urn:schemas-microsoft-com:xslt"
    exclude-result-prefixes="msxsl"
    xmlns:loglevel="urn:loglevel"
>
    <msxsl:script language="CSharp" implements-prefix="loglevel">
        <![CDATA[
        public int parseLevel(int val)
        {
            if ((val & 0x08) == 0x08) { return 4; }
            if ((val & 0x04) == 0x04) { return 3; }
            if ((val & 0x02) == 0x02) { return 2; }
            if ((val & 0x01) == 0x01) { return 1; }
            return 0;
        }
        ]]>
    </msxsl:script>
  <xsl:output method="html" indent="yes"/>

  <xsl:template match="VoodooLog">
    <html>
      <title>Voodoo Shader Framework Log</title>
      <link rel="stylesheet" href="VoodooLog.css" media="all" />
      <body>
        <div class="section">
          <div class="section_title">Log Information:</div>
          <xsl:variable name="runtime" select="child::Message[last()]/@ticks - /VoodooLog/@ticks" />
          Log file was started on <xsl:value-of select="/VoodooLog/@date" /> at <xsl:value-of select="/VoodooLog/@time" />;
          contains <xsl:value-of select="count(child::Message)" /> entries over a run time of
          <xsl:choose>
            <xsl:when test="$runtime &gt;= 60000">
              <xsl:variable name="minutes" select="floor($runtime div 60000)" />
              <xsl:value-of select="$minutes" /> minutes and <xsl:value-of select="round(($runtime mod 60000) div 1000)" /> seconds
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$runtime div 1000" /> seconds
            </xsl:otherwise>
          </xsl:choose>
          (<xsl:value-of select="$runtime" /> ticks).
        </div>
        <div class="section">
          <div class="section_title">Loaded Modules:</div>
          <table class="sortable">
            <thead>
              <tr>
                <th>Order</th>
                <th>Name</th>
                <th>Uuid</th>
                <th>Major</th>
                <th>Minor</th>
                <th>Patch</th>
                <th>Revision</th>
                <th>Debug</th>
              </tr>
            </thead>
            <xsl:for-each select="Module">
              <tr>
                <td>
                  <xsl:value-of select="position()" />
                </td>
                <td>
                  <xsl:value-of select="@name" />
                </td>
                <td>
                  <xsl:value-of select="@uuid" />
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
                <td>
                  <xsl:value-of select="@debug" />
                </td>
              </tr>
            </xsl:for-each>
          </table>
        </div>
        <div class="section">
          <div class="section_title">Log Events:</div>
          <table class="sortable" >
            <thead>
              <tr>
                <th>Event</th>
                <th>Severity</th>
                <th>Ticks</th>
                <th>Source</th>
                <th>Body</th>
              </tr>
            </thead>
            <xsl:for-each select="Message">
              <xsl:variable name="level" select="loglevel:parseLevel(@severity)" />
              <xsl:text disable-output-escaping="yes">&lt;tr class="level_</xsl:text>
              <xsl:value-of select="$level" />
              <xsl:text disable-output-escaping="yes">"&gt;</xsl:text>
              <td>
                <xsl:value-of select="position()" />
              </td>
              <td sorttable_customkey="{$level}">
                <xsl:value-of select="$level" />
              </td>
              <td>
                <xsl:value-of select="@ticks - /VoodooLog/@ticks" />
                (<xsl:choose>
                    <xsl:when test="position() = 1">
                        <xsl:value-of select="@ticks - /VoodooLog/@ticks" />
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:value-of select="@ticks - current()/preceding-sibling::Message[position()=1]/@ticks" />
                    </xsl:otherwise>
                </xsl:choose>)
              </td>
              <td>
                <xsl:value-of select="@source" />
              </td>
              <td>
                <xsl:value-of disable-output-escaping="yes" select="." />
              </td>
              <xsl:text disable-output-escaping="yes">&lt;/tr&gt;</xsl:text>
            </xsl:for-each>
          </table>
        </div>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>