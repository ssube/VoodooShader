<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:msxsl="urn:schemas-microsoft-com:xslt" exclude-result-prefixes="msxsl"
>
    <xsl:output method="html" indent="yes"/>
    <xsl:template match="VoodooRemote">
        <html background="#880000">
            <head>
                <title>Voodoo Remote Manifest</title>
            </head>
            <body>
                <ul><xsl:for-each select="child::Package">
                    <li>
                        <xsl:value-of disable-output-escaping="yes" select="." />
                    </li>
                </xsl:for-each></ul>
            </body>
        </html>
    </xsl:template>
    <xsl:template match="PackageManifest">
        <html>
            <head>
                <title>Voodoo Package Manifest</title>
            </head>
            <body>
                <div>
                    <div>Package: </div>
                    <xsl:apply-templates select="Package" />
                </div>
                <div>
                    <div>Description: </div>
                    <xsl:value-of select="Description" />
                </div>
                <div>
                    <div>PackageUri: </div>
                    <xsl:value-of select="PackageUri" />
                </div>
                <div>
                    <div>Messages:</div>
                    <xsl:for-each select="Messages/TranslationSet">
                        <xsl:apply-templates select="." />
                    </xsl:for-each>
                </div>
                <div>
                    <div>Versions: </div>
                    <xsl:for-each select="Versions/Version">
                        <xsl:apply-templates select="." />
                    </xsl:for-each>
                </div>
            </body>
        </html>
    </xsl:template>
</xsl:stylesheet>
