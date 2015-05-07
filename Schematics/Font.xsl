<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- Generate ANSI C code for proportional fonts-->
<!-- Histroy:-->
<!-- 03.04.2010 Bug Fix [FB]: Changed absolut to relativ path in template recursivLength to get bytesperbitmap -->

<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:xs="http://www.w3.org/2001/XMLSchema">
	<xsl:output method="text" encoding="ISO-8859-1"/>
	<xsl:strip-space elements="*"/>

	<xsl:template match="/">
		 <xsl:apply-templates select="//project"/>
		 <xsl:apply-templates select="//project/group"/>
	</xsl:template>

	<!-- Generate Preambel -->
	<xsl:template match="project">
		<xsl:text>/*&#xa;</xsl:text>
		<xsl:text>YAAB - Yet Another Autococker Board&#xa;</xsl:text>
		<xsl:text>Arduino based autococker board developed around the platform and ATMEL AVR&#xa;</xsl:text>
		<xsl:text>chips&#xa;</xsl:text>
		<xsl:text>&#xa;</xsl:text>
		<xsl:text>Copyright (C) 2013  Dan Silk&#xa;</xsl:text>
		<xsl:text>&#xa;</xsl:text>
		<xsl:text>This program is free software: you can redistribute it and/or modify&#xa;</xsl:text>
		<xsl:text>it under the terms of the GNU General Public License as published by&#xa;</xsl:text>
		<xsl:text>the Free Software Foundation, either version 3 of the License, or&#xa;</xsl:text>
		<xsl:text>(at your option) any later version.&#xa;</xsl:text>
		<xsl:text>&#xa;</xsl:text>
		<xsl:text>This program is distributed in the hope that it will be useful,&#xa;</xsl:text>
		<xsl:text>but WITHOUT ANY WARRANTY; without even the implied warranty of&#xa;</xsl:text>
		<xsl:text>MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the&#xa;</xsl:text>
		<xsl:text>GNU General Public License for more details.&#xa;</xsl:text>
		<xsl:text>&#xa;</xsl:text>
		<xsl:text>You should have received a copy of the GNU General Public License&#xa;</xsl:text>
		<xsl:text>along with this program.  If not, see &lt;http://www.gnu.org/licenses/&gt;.&#xa;</xsl:text>
		<xsl:text>*/&#xa;</xsl:text>
		<xsl:text>&#xa;</xsl:text>
		<xsl:text>//--------------------------------------------------------------&#xa;</xsl:text>
		<xsl:text>//               Font and Bitmap Generator                      &#xa;</xsl:text>
		<xsl:text>//                                                              &#xa;</xsl:text>
		<xsl:text>// 					   www.muGUI.de                           &#xa;</xsl:text> 
		<xsl:text>//                                                              &#xa;</xsl:text>
		<xsl:text>// Version:   </xsl:text>
		<xsl:value-of select="version"/> 
		<xsl:text>&#xa;</xsl:text>
		<xsl:text>// Project Name:  </xsl:text>
		<xsl:value-of select="projectname"/> 
		<xsl:text>&#xa;</xsl:text>	
		<xsl:text>//--------------------------------------------------------------&#xa;</xsl:text>
		<xsl:text>// Generated by mugui2c.xsl, Copyright 07.11.2009               &#xa;</xsl:text>
		<xsl:text>//--------------------------------------------------------------&#xa;</xsl:text>
		<xsl:text>&#xa;</xsl:text>
		<xsl:text>#include &lt;avr/pgmspace.h&gt;</xsl:text>
		<xsl:text>&#xa;</xsl:text>
		<xsl:text>&#xa;</xsl:text>
	</xsl:template> 


	<xsl:template match="/project/group">
		<xsl:variable name="StopCharacter">
			<xsl:value-of select="startcharacter + numberofbitmaps -1"/>
		</xsl:variable>
		<xsl:variable name="Bytesperbitmap">
			<xsl:value-of select="bytesperbitmapline"/>
		</xsl:variable>
		<xsl:variable name="Width">
			<xsl:value-of select="width"/>
		</xsl:variable>
		<xsl:variable name="Index">0</xsl:variable>


		<xsl:text>//--------------------------------------------------------------&#xa;</xsl:text>
		<xsl:text>// Group Name: </xsl:text>
		<xsl:value-of select="groupname"/>
		<xsl:text>&#xa;</xsl:text>
		<xsl:text>// Designer: </xsl:text>
		<xsl:value-of select="designer"/>
		<xsl:text>&#xa;</xsl:text>
		<xsl:text>// Date: </xsl:text>
		<xsl:value-of select="date"/>
		<xsl:text>&#xa;</xsl:text>
		<xsl:text>// Description: </xsl:text>
		<xsl:value-of select="description"/>
		<xsl:text>&#xa;</xsl:text>
		<xsl:text>// Byteorientation: </xsl:text>
		<xsl:value-of select="byteorientation"/>
		<xsl:text>&#xa;</xsl:text>
		<xsl:text>// Font Height: </xsl:text>
		<xsl:value-of select="height"/>
		<xsl:text> Pixel&#xa;</xsl:text>
		<xsl:if test="contains(proportional,'True')">
			<xsl:text>// Proportional font &#xa;</xsl:text>
			<xsl:text>// Font Width: Individual &#xa;</xsl:text>
		</xsl:if>
		<xsl:if test="contains(proportional,'False')">
			<xsl:text>// Monospace font &#xa;</xsl:text>
			<xsl:text>// Font Width: </xsl:text>
			<xsl:value-of select="width"/>
			<xsl:text> Pixel&#xa;</xsl:text>
		</xsl:if>
		<xsl:text>// Number of Bitmaps: </xsl:text>
		<xsl:value-of select="numberofbitmaps"/>
		<xsl:text>&#xa;</xsl:text>
		<xsl:if test="contains(proportional,'False')">
			<xsl:text>// Bytes per Bitmap: </xsl:text>
			<xsl:value-of select="bytesperbitmap"/>
			<xsl:text>&#xa;</xsl:text>
		</xsl:if>
		<xsl:text>// Start Character: </xsl:text>
		<xsl:value-of select="startcharacter"/>
		<xsl:text>&#xa;</xsl:text>
		<xsl:text>// Stop Character: </xsl:text>
		<xsl:value-of select="$StopCharacter"/>
		<xsl:text>&#xa;</xsl:text>
		<xsl:text>//---------------------------------------------------------------&#xa;</xsl:text>
		
		<!-- Make Header -->
		<xsl:text>PROGMEM const unsigned char </xsl:text>
		<xsl:value-of select="groupname"/>
		<xsl:text>[] = &#xa;{&#xa;</xsl:text>
		<xsl:text>&#9;// Font Header &#xa;</xsl:text>
		
		<xsl:if test="contains(proportional,'True')">
			<xsl:text>&#9;0x01,</xsl:text>
		</xsl:if>
		<xsl:if test="contains(proportional,'False')">
			<xsl:text>&#9;0x00,</xsl:text>
		</xsl:if>
		<xsl:text>&#9;// 0x00: Monospace, 0x01: Proportional &#xa;</xsl:text>
		
		<xsl:if test="contains(byteorientation,'horizontalLeft')">
			<xsl:text>&#9;0x00,</xsl:text>
		</xsl:if>
		<xsl:if test="contains(byteorientation,'horizontalRight')">
			<xsl:text>&#9;0x01,</xsl:text>
		</xsl:if>
		<xsl:if test="contains(byteorientation,'verticalCeiling')">
			<xsl:text>&#9;0x02,</xsl:text>
		</xsl:if>
		<xsl:if test="contains(byteorientation,'verticalBottom')">
			<xsl:text>&#9;0x03,</xsl:text>
		</xsl:if>
		<xsl:text>&#9;// 0x00: horizontalLeft,  0x01: horizontalRight &#xa;</xsl:text>
		<xsl:text>&#9;&#9;&#9;// 0x02: verticalCeiling, 0x03: verticalBottom &#xa;</xsl:text>
		
		<xsl:text>&#9;0x</xsl:text>
		<xsl:call-template name="toHex">
			<xsl:with-param name="decimalNumber" select="startcharacter" />
		</xsl:call-template>
		<xsl:text>,&#9;// Startcharacter &#xa;</xsl:text>
		
		<xsl:text>&#9;0x</xsl:text>
		<xsl:call-template name="toHex">
			<xsl:with-param name="decimalNumber" select="numberofbitmaps" />
		</xsl:call-template>
		<xsl:text>,&#9;// Number of Bitmaps &#xa;</xsl:text>
		
		<xsl:text>&#9;0x</xsl:text>
		<xsl:call-template name="toHex">
			<xsl:with-param name="decimalNumber" select="height" />
		</xsl:call-template>
		<xsl:text>,&#9;// Height &#xa;</xsl:text>
		
		<xsl:if test="contains(proportional,'False')">
			<xsl:text>&#9;0x</xsl:text>
			<xsl:call-template name="toHex">
				<xsl:with-param name="decimalNumber" select="width" />
			</xsl:call-template>
		<xsl:text>,&#9;// Width &#xa;</xsl:text>
		</xsl:if>
		
		<xsl:text>&#xa;&#xa;</xsl:text>
		
		<xsl:if test="contains(proportional,'True')">
			<!-- Make Index -->
			<xsl:call-template name="makeIndex"/>

			<!-- Make Bitmaps -->
			<xsl:for-each select="bitmap">
				<xsl:text>&#9;/* Character Data (width: </xsl:text>
					<xsl:value-of select="bitmapwidth"/>
				<xsl:text>  bit) - Index: </xsl:text>
				<xsl:value-of select="position()-1"/>
				<xsl:text> */&#xa;</xsl:text>
				<xsl:text>&#9;0x</xsl:text>

				<xsl:call-template name="toHex">
					<xsl:with-param name="decimalNumber" select="bitmapwidth" />
				</xsl:call-template>
				<xsl:text>, </xsl:text>
				<xsl:for-each select="byte">
					<xsl:value-of select="."/>
					<xsl:if test="position()!=last()">
						<xsl:text>,</xsl:text>
					</xsl:if>
				</xsl:for-each>
				<xsl:if test="position()!=last()">
					<xsl:text>,&#xa;&#xa;</xsl:text>
				</xsl:if>
			</xsl:for-each>
		</xsl:if>
		<xsl:if test="contains(proportional,'False')">
			<!-- Make Bitmaps -->
			<xsl:for-each select="bitmap">
				<xsl:text>&#9;/* Character Data - Index: </xsl:text>
				<xsl:value-of select="position()-1"/>
				<xsl:text> */&#xa;</xsl:text>
				<xsl:text>&#9;</xsl:text>

				<xsl:for-each select="byte">
					<xsl:value-of select="."/>
					<xsl:if test="position()!=last()">
						<xsl:text>,</xsl:text>
					</xsl:if>
				</xsl:for-each>
				<xsl:if test="position()!=last()">
					<xsl:text>,&#xa;&#xa;</xsl:text>
				</xsl:if>
			</xsl:for-each>
		</xsl:if>
		
		<xsl:text>&#xa;};&#xa;</xsl:text>
	</xsl:template>

	
	<!-- Convert Dezimal to Hex --> 
	<xsl:variable name="hexDigits" select="'0123456789ABCDEF'" />
	<xsl:template name="toHex">
		<xsl:param name="decimalNumber" />
     	<xsl:if test="$decimalNumber >= 16">
			<xsl:call-template name="toHex">
				<xsl:with-param name="decimalNumber" select="floor($decimalNumber div 16)" />
			</xsl:call-template>
		</xsl:if>
		<xsl:value-of select="substring($hexDigits, ($decimalNumber mod 16) + 1, 1)" />
	</xsl:template>


	<!-- Make the index with start address of the bitmaps -->
	<xsl:template name="makeIndex">
		<xsl:text>&#9;// Bitmap Index: Number of Index Entries </xsl:text>
			<xsl:value-of  select="count(bitmap)"/>
		<xsl:text>&#xa; </xsl:text>
		
		<xsl:variable name="mcount" select="count(bitmap)*2"/>

		<xsl:for-each select="bitmap">
			<xsl:variable name="laenge">
				<xsl:call-template name="recursivLength">
					<xsl:with-param name="n">
						<xsl:value-of select="position()"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:variable>

			<!-- length of data bytes, Index length, Header (5 Bytes but starting with 0 ->2)  -->
			<xsl:variable name="Highbyte">
					<xsl:value-of select="floor(($laenge + $mcount + 5) div 256)"/>
			</xsl:variable>
			
					
			<xsl:variable name="Lowbyte">
					<xsl:value-of select="($laenge + $mcount + 5) mod 256"/>
			</xsl:variable>
			<xsl:text>&#9;0x</xsl:text>
			
			<xsl:call-template name="toHex">
				<xsl:with-param name="decimalNumber" select="$Highbyte" />
			</xsl:call-template>

			<xsl:text>, 0x</xsl:text>
			
			<xsl:call-template name="toHex">
				<xsl:with-param name="decimalNumber" select="$Lowbyte" />
			</xsl:call-template>

			<xsl:text>, //Index Nummer: </xsl:text>
				<xsl:value-of select="position()-1"/>
			<xsl:text>&#xa;</xsl:text>
		</xsl:for-each>
		<xsl:text>&#xa;</xsl:text>
	</xsl:template>

	<!-- Calculate the bitmap bytes from bitmap 1 to bitmap n -->
	<!-- Remember: XSLT starts indexing with 1 and not with 0 -->
	<xsl:template name="recursivLength">
	   <xsl:param name="n"/>
	   <xsl:if test="$n > 1"> 
				<xsl:variable name="Width" >
					<xsl:call-template name="recursivLength">
						<xsl:with-param name="n" select="$n - 1"/>
					</xsl:call-template>
				</xsl:variable> 
				<xsl:variable name="BytesPerBitmap">
					<xsl:value-of select="../bitmap[$n]/bytesperbitmap" />
				</xsl:variable>
				<xsl:value-of select="$Width + $BytesPerBitmap + 1" />
	   </xsl:if>
	   <xsl:if test="$n = 1"> 
			<xsl:variable name="Width" >0</xsl:variable> 
			<xsl:value-of select="$Width" />
	   </xsl:if>
	</xsl:template>


 </xsl:stylesheet>