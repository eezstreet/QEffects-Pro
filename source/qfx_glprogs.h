/***************************************************************************
* Copyright (C) 2012, Chain Studios.
* 
* This file is part of QeffectsGL source code.
* 
* QeffectsGL source code is free software; you can redistribute it 
* and/or modify it under the terms of the GNU General Public License 
* as published by the Free Software Foundation; either version 2 of 
* the License, or (at your option) any later version.
* 
* QeffectsGL source code is distributed in the hope that it will be 
* useful, but WITHOUT ANY WARRANTY; without even the implied 
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
* See the GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
***************************************************************************/
#ifndef QFX_GLPROGS_H
#define QFX_GLPROGS_H

#define _NL "\n"

static const char g_szVertexShader_DOFDepth[] = 
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_DOFDepth[] = 
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform vec4 Local0;\n"
"uniform sampler2DRect Texture0;\n"
"float read_depth( vec2 coord )\n"
"{\n"
"return Local0.x / (Local0.y + Local0.x - texture2DRect( Texture0, coord ).r * Local0.y);\n"
"}\n"
"void main(void)\n"
"{\n"
"gl_FragColor.rgb = read_depth( gl_TexCoord[0].xy );\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

// Hey look, ANOTHER box blur shader >_>
static const char g_szVertexShader_DOFBlur[] = 
"uniform vec4 Local0;\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0 + Local0;\n"
"gl_TexCoord[1] = gl_MultiTexCoord0 + Local0 * 2.0;\n"
"gl_TexCoord[2] = gl_MultiTexCoord0 - Local0;\n"
"gl_TexCoord[3] = gl_MultiTexCoord0 - Local0 * 2.0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_DOFBlur[] = 
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect Texture0;\n"
"void main(void)\n"
"{\n"
"vec3 col1 = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n" // you mean this? i think so yes
"vec3 col2 = texture2DRect(Texture0, gl_TexCoord[1].xy).rgb;\n" // y isn't affected by the above. so it is identical for all 4 of them
"vec3 col3 = texture2DRect(Texture0, gl_TexCoord[2].xy).rgb;\n" // only x is different.
"vec3 col4 = texture2DRect(Texture0, gl_TexCoord[3].xy).rgb;\n" // so what's the point of the local then? you can change the vector to make it affect it differnetly. The way it is now, it only does horizontal. But you can change it to 0.5 0.5 0 and it will do vertical and horizontal. But slightly closer samples
"gl_FragColor.rgb = (col1 + col2 + col3 + col4) / 4.0;\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_DOFRender[] = 
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0.xyxy * vec4( 1.0, 1.0, 0.5, 0.5 );\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_DOFRender[] = 
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform vec4 Local0;\n"
"uniform sampler2DRect Texture0;\n"
"uniform sampler2DRect Texture1;\n"
"uniform sampler2DRect Texture2;\n"
"void main(void)\n"
"{\n"
"float ratio;\n"
"vec3 colNormal = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"vec3 colBlur = texture2DRect(Texture1, gl_TexCoord[0].zw).rgb;\n"
"float depth = texture2DRect(Texture2, gl_TexCoord[0].zw).r;\n"
"if ( depth < Local0.x || depth == 1.0 ) ratio = 0.0;\n"
"else ratio = clamp( abs(depth - Local0.y) * Local0.z, 0.0, 1.0 );\n"
"gl_FragColor.rgb = mix( colNormal, colBlur, ratio * pow( depth, 0.25 ) );\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_Emboss[] = 
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_Emboss[] = 
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect Texture0;\n"
"uniform vec4 Local0;\n"
"void main(void)\n"
"{\n"
"vec3 col1 = texture2DRect(Texture0, gl_TexCoord[0].xy - vec2( 1.0, 1.0 )).rgb;\n"
"vec3 col2 = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"vec3 col3 = texture2DRect(Texture0, gl_TexCoord[0].xy + vec2( 1.0, 1.0 )).rgb;\n"
"vec3 colEmboss = col1 * 2.0 - col2 - col3;\n"
"float colDot = max( 0.0, dot( colEmboss, vec3( 0.333, 0.333, 0.333 ) ) ) * Local0.x;\n"
"vec3 colFinal = col2 - vec3( colDot ) ;\n"
"float luminance = dot( col2, vec3( 0.6, 0.2, 0.2 ) );\n"
"gl_FragColor.rgb = mix( colFinal, col2, luminance * luminance );\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_HSL[] = 
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

// eezstreet: The old HSL shader was absolute GARBAGE. replacing it with this seems like a fairly wise choice
static const char g_szFragmentShader_HSL[] = 
	"#extension GL_ARB_texture_rectangle : enable" _NL
	"uniform vec4 Local0;"						_NL
	"uniform sampler2DRect Texture0;"			_NL

	// Convert hue to RGB color
	"float HueToRGB(float v1, float v2, float vH)"				_NL
	"{"															_NL
		"float ret;"											_NL
		"if( vH < 0.0 ) vH += 1.0;"								_NL
		"if( vH > 1.0 ) vH -= 1.0;"								_NL
		"if( ( 6.0 * vH ) < 1.0 )"								_NL
			"ret = ( v1 + (v2 - v1 ) * 6.0 * vH );"				_NL
		"else if( ( 2.0 * vH ) < 1.0)"							_NL
			"ret = ( v2 );"										_NL
		"else if( ( 3.0 * vH ) < 2.0)"							_NL
			"ret = ( v1 + ( v2 - v1 )*((2.0/3.0)-vH)*6.0);"		_NL
		"else"													_NL
			"ret = v1;"											_NL
		"return ret;"											_NL
	"}"															_NL

	// Main entry point of shader
	"void main (void)"											_NL
	"{"															_NL
		"vec3 color;"											_NL

		"float Cmax, Cmin;"										_NL

		"float D;"												_NL
		"float H, S, L;"										_NL
		"float R, G, B;"										_NL
		// Get fragment color
		"color = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;"						_NL

		"R = color.r;"											_NL
		"G = color.g;"											_NL
		"B = color.b;"											_NL

		// Convert to HSL
		"Cmax = max (R, max(G,B));"								_NL
		"Cmin = min (R, min(G,B));"								_NL

		// Calculate lightness
		"L = (Cmax + Cmin) / 2.0;"								_NL

		"if (Cmax == Cmin)"										_NL
		// It's grey
		"{"														_NL
			"H = 0.0;"											_NL
			"S = 0.0;"											_NL
		"}"														_NL
		"else"													_NL
		"{"														_NL
		// we know D != 0 so we can safely divide by it
			"D = Cmax - Cmin;"									_NL
		
			// Calculate saturation
			"if (L < 0.5)"											_NL
			"{"														_NL
				"S = D / (Cmax + Cmin);"							_NL
			"}"														_NL
			"else"													_NL
			"{"														_NL
				"S = D / (2.0 - (Cmax + Cmin));"					_NL
			"}"														_NL
			// Calculate hue
			"if (R == Cmax)"									_NL
			"{"													_NL
				"H = (G - B) / D;"								_NL
			"}"													_NL
			"else"												_NL
			"{"													_NL
				"if(G == Cmax)"									_NL
				"{"												_NL
					"H = 2.0 + (B - R) / D;"					_NL
				"}"												_NL
				"else"											_NL
				"{"												_NL
					"H = 4.0 + (R - G) / D;"					_NL
				"}"												_NL
			"}"													_NL
			"H = H / 6.0;"										_NL
		"}"														_NL

		// Modify H/S/L values
		"H += Local0.x;"									_NL
		"S += Local0.y;"									_NL
		"L += Local0.z;"									_NL

		"if (H < 0.0)"										_NL
		"{"													_NL
			"H = H + 1.0;"									_NL
		"}"													_NL

		// Clamp HSL to 0-1
		"H = clamp(H, 0.0, 1.0);"							_NL
		"S = clamp(S, 0.0, 1.0);"							_NL
		"L = clamp(L, 0.0, 1.0);"							_NL

		// Convert back to RGB
		"float var_2, var_1;"								_NL
		
		"if (S == 0.0)"										_NL
		"{"													_NL
			"R = L;"										_NL
			"G = L;"										_NL
			"B = L;"										_NL
		"}"													_NL
		"else"												_NL
		"{"													_NL
			"if ( L < 0.5 )"								_NL
			"{"												_NL
				"var_2 = L * (1.0 + S );"					_NL
			"}"												_NL
			"else"											_NL
			"{"												_NL
				"var_2 = ( L + S ) - ( S * L );"			_NL
			"}"												_NL

			"var_1 = 2.0 * L - var_2;"						_NL

			"R = HueToRGB(var_1, var_2, H + ( 1.0 / 3.0 ));"_NL
			"G = HueToRGB(var_1, var_2, H);"				_NL
			"B = HueToRGB(var_1, var_2, H - ( 1.0 / 3.0 ));"_NL
		"}"													_NL
		"gl_FragColor = vec4(R,G,B, 1.0);"				_NL
	"}"
;

static const char g_szVertexShader_SSAORender[] = 
"uniform vec4 Local0;\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0.xyxy * vec4( 1.0, 1.0, 1.0 / Local0.z, 1.0 / Local0.w );\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_SSAORender[] = 
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform vec4 Local0;\n"
"uniform vec4 Local1;\n"
"uniform sampler2DRect Texture0;\n"
"float read_depth( vec2 coord )\n"
"{\n"
"return Local0.x / (Local0.y + Local0.x - texture2DRect( Texture0, coord ).r * Local0.y);\n"
"}\n"
"vec2 rand( vec2 coord )\n"
"{\n"
"float coordDot = dot( coord, vec2( 12.9898, 78.233 ) );\n"
"float noiseX = fract( sin( coordDot ) * 43758.5453 );\n"
"float noiseY = fract( sin( coordDot * 2.0 ) * 43758.5453 );\n"
"return vec2( noiseX, noiseY ) * 0.004;\n"
"}\n"
"float compare_depth( float depth1, float depth2 )\n"
"{\n"
"float aoRange = 60.0;\n"
"float depthDiff = depth1 - depth2;\n"
"float propagation = depthDiff * (Local0.y - Local0.x);\n"
"float rangeDiff = sqrt(clamp( 1.0 - propagation / aoRange, 0.0, 1.0 ));\n"
"float ao = Local1.z * depthDiff * (2.0 - depthDiff) * rangeDiff;\n"
"return ao;\n"
"}\n"
"void main(void)\n"
"{\n"
"float depth = read_depth( gl_TexCoord[0].xy );\n"
"float ao;\n"
"if ( depth > Local1.x && depth < Local1.y ) {\n"
"vec2 noise = rand( gl_TexCoord[0].zw );\n"
"float distScale = 1.0 + 5.0 * sqrt( 1.0 - depth );\n"
"float w = distScale + (noise.x*(1.0-noise.x)) * Local0.z;\n"
"float h = distScale + (noise.y*(1.0-noise.y)) * Local0.w;\n"
"vec2 ofs;\n"
"float d;\n"
"int samples = 3;\n"
"int rings = 3;\n"
"float TWO_PI = 2.0 * 3.14159265;\n"
"for ( int i = 1 ; i <= rings; i++ ) {\n"
"float angleStep = TWO_PI / float( samples * i );\n"
"for ( int j = 1 ; j <= samples*i; j++ ) {\n"
"ofs.x = cos( float(j) * angleStep ) * float(i) * w;\n"
"ofs.y = sin( float(j) * angleStep ) * float(i) * h;\n"
"d = read_depth( gl_TexCoord[0].xy + ofs );\n"
"// ignore occluder with too low depth value (possible viewmodel)\n"
"if ( d < Local1.x ) continue;\n"
"ao += compare_depth( depth, d );\n"
"}\n"
"}\n"
"ao = 1.0 - ao;\n"
"} else {\n"
"ao = 1.0;\n"
"}\n"
"gl_FragColor.rgb = vec3( ao );\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_SSAOBlur[] = 
"uniform vec4 Local0;\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0 + Local0;\n"
"gl_TexCoord[1] = gl_MultiTexCoord0 + Local0 * 2.0;\n"
"gl_TexCoord[2] = gl_MultiTexCoord0 - Local0;\n"
"gl_TexCoord[3] = gl_MultiTexCoord0 - Local0 * 2.0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_SSAOBlur[] = 
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect Texture0;\n"
"void main(void)\n"
"{\n"
"vec3 col1 = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"vec3 col2 = texture2DRect(Texture0, gl_TexCoord[1].xy).rgb;\n"
"vec3 col3 = texture2DRect(Texture0, gl_TexCoord[2].xy).rgb;\n"
"vec3 col4 = texture2DRect(Texture0, gl_TexCoord[3].xy).rgb;\n"
"gl_FragColor.rgb = (col1 + col2 + col3 + col4) / 4.0;\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_SSAOCombine[] = 
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"gl_TexCoord[1] = gl_MultiTexCoord0 * 0.5;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_SSAOCombine[] = 
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect Texture0;\n"
"uniform sampler2DRect Texture1;\n"
"void main(void)\n"
"{\n"
"vec3 col1 = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"vec3 col2 = texture2DRect(Texture1, gl_TexCoord[1].xy).rgb;\n"
"float luminance = dot( col1, vec3( 0.6, 0.2, 0.2 ) );\n"
"gl_FragColor.rgb = mix( col1 * col2, col1, luminance * luminance );\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_BloomDarken[] = 
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_BloomDarken[] = 
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform vec4 Local0;\n"
"uniform sampler2DRect Texture0;\n"
"void main(void)\n"
"{\n"
"vec3 col = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"gl_FragColor.rgb = pow( col, Local0.xxx );\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_BloomBlur[] = 
"uniform vec4 Local0;\n"
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0 + Local0;\n"
"gl_TexCoord[1] = gl_MultiTexCoord0 + Local0 * 2.0;\n"
"gl_TexCoord[2] = gl_MultiTexCoord0 - Local0;\n"
"gl_TexCoord[3] = gl_MultiTexCoord0 - Local0 * 2.0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_BloomBlur[] = 
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect Texture0;\n"
"void main(void)\n"
"{\n"
"vec3 col1 = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"vec3 col2 = texture2DRect(Texture0, gl_TexCoord[1].xy).rgb;\n"
"vec3 col3 = texture2DRect(Texture0, gl_TexCoord[2].xy).rgb;\n"
"vec3 col4 = texture2DRect(Texture0, gl_TexCoord[3].xy).rgb;\n"
"gl_FragColor.rgb = (col1 + col2 + col3 + col4) / 4.0;\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";

static const char g_szVertexShader_BloomCombine[] = 
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"gl_TexCoord[1] = gl_MultiTexCoord0 * 0.5;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_BloomCombine[] = 
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform vec4 Local0;\n"
"uniform sampler2DRect Texture0;\n"
"uniform sampler2DRect Texture1;\n"
"void main(void)\n"
"{\n"
"vec3 col1 = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"vec3 col2 = texture2DRect(Texture1, gl_TexCoord[1].xy).rgb;\n"
"gl_FragColor.rgb = col1 + col2 * Local0.x;\n"
"gl_FragColor.a	= 1.0;\n"
"}\n";


// eezstreet add: Motion blur

// renamed original g_szVertexShader_MotionBlur to g_szVertexShader_MotionBlurOLD
static const char g_szVertexShader_MotionBlur[] = 
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
//"gl_TexCoord[1] = gl_MultiTexCoord0 * 0.5;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_MotionBlur[] = 
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect Texture0;\n"
"uniform sampler2DRect Texture1;\n"
"uniform sampler2DRect Texture2;\n" 
"uniform sampler2DRect Texture3;\n" 
//"uniform sampler2DRect Texture4;\n"
//"uniform sampler2DRect Texture5;\n"
//"uniform sampler2DRect Texture6;\n"
//"uniform sampler2DRect Texture7;\n"
"uniform int Local0;\n"
"void main(void)\n"
"{\n"
//"vec3 colors[8];\n"
"vec3 colors[4];\n"
"colors[0] = texture2DRect(Texture0, gl_TexCoord[0].xy).rgb;\n"
"colors[1] = texture2DRect(Texture1, gl_TexCoord[0].xy).rgb;\n"
"colors[2] = texture2DRect(Texture2, gl_TexCoord[0].xy).rgb;\n"
"colors[3] = texture2DRect(Texture3, gl_TexCoord[0].xy).rgb;\n"
//"colors[4] = texture2DRect(Texture4, gl_TexCoord[0].xy).rgb;\n"
//"colors[5] = texture2DRect(Texture5, gl_TexCoord[0].xy).rgb;\n"
//"colors[6] = texture2DRect(Texture4, gl_TexCoord[0].xy).rgb;\n"
//"colors[7] = texture2DRect(Texture5, gl_TexCoord[0].xy).rgb;\n"
/*"gl_FragColor.rgb += colors[Local0%8] * 0.125;\n"
"gl_FragColor.rgb += colors[Local0+1%8] * 0.125;\n"
"gl_FragColor.rgb += colors[Local0+2%8] * 0.125;\n"
"gl_FragColor.rgb += colors[Local0+3%8] * 0.125;\n"
"gl_FragColor.rgb += colors[Local0+4%8] * 0.125;\n"
"gl_FragColor.rgb += colors[Local0+5%8] * 0.125;\n"
"gl_FragColor.rgb += colors[Local0+6%8] * 0.125;\n"
"gl_FragColor.rgb += colors[Local0+7%8] * 0.125;\n"*/
"gl_FragColor.rgb += colors[Local0%4] * 0.25;\n"
"gl_FragColor.rgb += colors[Local0+1%4] * 0.25;\n"
"gl_FragColor.rgb += colors[Local0+2%4] * 0.25;\n"
"gl_FragColor.rgb += colors[Local0+3%4] * 0.25;\n"

"gl_FragColor.a	= 1.0;\n"
"}\n";

// Anaglyph 3D

static const char g_szVertexShader_Anaglyph[] =
"#extension GL_ARB_texture_rectangle : enable\n"

"void main()\n"
"{\n"
"    gl_Position = ftransform();\n"
"    gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"}\n";

static const char g_szFragmentShader_Anaglyph[] = 
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect Texture0;\n"

"void main(void)\n"
"{\n"
"    float AnaglyphSeperation = 8.0;\n" // UQ1: Hard coded for now...
"    float AnaglyphRed = 0.0;\n" // UQ1: Hard coded for now...
"    float AnaglyphGreen = 0.0;\n" // UQ1: Hard coded for now...
"    float AnaglyphBlue = 0.0;\n" // UQ1: Hard coded for now...
"    vec4 Anaglyph = texture2DRect(Texture0, gl_TexCoord[0].st).rgba;\n"
    // Setting RGB channel colors
"    float red = dot(Anaglyph.rgb, vec4(2.55, 0, 0, 0));\n"
"    float green = dot(Anaglyph.rgb, vec4(0,2.55,0, 0));\n"
"    float blue = dot(Anaglyph.rgb, vec4(0,0,2.55,0));\n"

    // Setting the RGB channel powers
"    vec4 red2 = red * 0.111;\n"
"    vec4 green2 = green * 0.111;\n"
"    vec4 blue2 = blue * 0.111;\n"

	// Left Eye (Red)
"    vec4 LeftEye = texture2DRect(Texture0, vec2(gl_TexCoord[0].st + vec2(-AnaglyphSeperation,0))).rgba;\n"
"    red2 = max(red2, LeftEye) - AnaglyphRed;\n"

    // Right Eye (Cyan)
"    vec4 RightEye = texture2DRect(Texture0, vec2(gl_TexCoord[0].st + vec2(AnaglyphSeperation,0))).rgba;\n"
"    green2 = max(green2, RightEye) - AnaglyphGreen;\n"
"    blue2 = max(blue2, RightEye) - AnaglyphBlue;\n"
"    vec4 cyan = (green2 + blue2) / 2.0;\n"

    // Combine
"    Anaglyph.r = cyan.r;\n"
"    Anaglyph.g = red2.g;\n"
"    Anaglyph.b = red2.b;\n"
"    Anaglyph.a = 1.0;\n"

"    gl_FragColor = Anaglyph;\n"
"}\n";

// eezstreet add: Vignette
static const char g_szVertexShader_Vignette[] = 
"void main(void)\n"
"{\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

static const char g_szFragmentShader_Vignette[] = 
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2DRect Texture0;\n"
"uniform vec4 Local0;\n"

"void main(void)\n"
"{\n"
"vec4 screen = texture2DRect( Texture0, gl_TexCoord[0].st );"
"float dist = length( gl_TexCoord[0].st - vec2( 0.5 ) );\n"
"dist *= dist * Local0.x;\n"
"gl_FragColor = vec4( screen.rgb * (1.0 - dist), 1.0 );\n"
"}\n";


#endif //QFX_GLPROGS_H
