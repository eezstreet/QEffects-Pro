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
#ifndef QFX_RENDERER_H
#define QFX_RENDERER_H

//#define QFX_MOTIONBLUR_SAMPLES		8			// make sure you alter qfx_glprogs.h!
#define QFX_MOTIONBLUR_SAMPLES		4			// make sure you alter qfx_glprogs.h!

class QFXShader;

typedef struct {
	int				m_varDebug;
	int				m_varCompareMode;
	int				m_varZTrickFix;
	int				m_varViewportFix;
	int				m_varRenderFrame;
	int				m_varAnisotropy;
	int				m_varMultisample;
	int				m_varMultisampleHint;
	int				m_varBloomEnable;
	int				m_varBloomNumSteps;
	float			m_varBloomDarkenPower;
	float			m_varBloomCombineScale;
	int				m_varSSAOEnable;
	float			m_varSSAOScale;
	float			m_varSSAOMinZ;
	float			m_varSSAOMaxZ;
	int				m_varHue;
	int				m_varSaturation;
	int				m_varLightness;
	int				m_varEmboss;
	float			m_varEmbossScale;
	int				m_varDOF;
	float			m_varDOFSpeed;
	float			m_varDOFScale;
	float			m_varDOFMinZ;
	// eezstreet add
	int				m_varMotionBlur;
	int				m_varAnaglyph;
	int				m_varVignette;
	float			m_varVignetteScale;
} QFXINIVars_t;

QFX_SINGLETON class QFXRenderer
{
private: 
	QFXRenderer();
	static QFXRenderer& GetInstance() { static QFXRenderer rObject; return rObject; }

public:
	static QFXRenderer& Instance() { typedef QFXRenderer& (*pfnGetInstance)(); static pfnGetInstance pf = &GetInstance; return pf(); }

	BOOL OnSetPixelFormat( HDC hdc, int pixelformat, CONST PIXELFORMATDESCRIPTOR *ppfd );
	void OnMakeCurrent( HDC hdc, HGLRC hglrc );
	void OnDeleteContext( HGLRC hglrc );
	void OnSwapBuffers( void );
	void OnTexImage2D( void );
	void SetOrtho( GLboolean enable );
	void SetupZDimensions( GLfloat znear, GLfloat zfar );
	void SetupZDimensions( const GLfloat* matrix );
	void SetupZDimensions( const GLdouble* matrix );
	void SetupDepthFunc( GLenum func );
	void SetupDepthRange( GLclampd zNear, GLclampd zFar );
	void SetupMultisample( void );
	void SetupAnisotropy( void );

private:
	GLuint AllocTextureId( void ) { return m_uiTextureIndex++; }
	int  ChooseMultisamplePixelFormat( int fmt, HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd );
	void CheckHardware( void );
	void InitializeGL( void );
	void ShutdownGL( void );
	void RenderGL( void );
	void CaptureRGB( void );
	void CaptureDepth( void );
	void RenderFSQ( int wide, int tall );
	void RenderScaledFSQ( int wide, int tall, int wide2, int tall2 );

	void PostProcess( void );
	void PostProcess_SSAO( void );
	void PostProcess_Bloom( void );
	void PostProcess_ColorCorrection( void );
	void PostProcess_Emboss( void );
	void PostProcess_DOF( void );
	void PostProcess_MotionBlur( void );	// eez add
	void PostProcess_Anaglyph( void );
	void PostProcess_Vignette( void );		// eez add

private:
	// contexts, internal state
	HDC				m_hDC;
	HGLRC			m_hGLRC;
	GLboolean		m_bOrtho;
	GLboolean		m_bZTrick;
	GLboolean		m_bInternalAnisotropy;
	GLboolean		m_bInternalMultisample;
	GLboolean		m_bMultisample;
	GLuint			m_uiRenderFrame;
	GLuint			m_uiRenderFrameCounter;
	GLuint			m_uiFrameCounter;
	GLuint			m_uiTextureIndex;
	GLint			m_WindowSize[2];
	GLfloat			m_zNear;
	GLfloat			m_zFar;
	DWORD			m_iEnableRender;
	GLfloat			m_flFrameTime;
	DWORD			m_flOldTime;
	DWORD			m_flLastTime;
	GLfloat			m_flLastDepth;

	// tracked GL state
	GLuint			m_uiViewportFrame;
	GLint			m_3DViewport[4];
	GLint			m_3DScissor[4];

	// hardware caps
	GLboolean		m_bSupportsBloom;
	GLboolean		m_bSupportsSSAO;
	GLboolean		m_bSupportsColorCorrection;
	GLboolean		m_bSupportsEmboss;
	GLboolean		m_bSupportsDOF;
	GLboolean		m_bSupportsMotionBlur;			// eez: add
	GLboolean		m_bSupportsAnaglyph;
	GLboolean		m_bSupportsVignette;			// eez: add

	// renderer settings
	QFXINIVars_t	m_iniVars;

	// renderer data
	GLuint			m_uiScreenRGB;
	GLuint			m_uiScreenDepth;
	GLuint			m_uiBlurTexture;
	GLuint			m_uiBlurDepth;
	QFXShader*		m_pBloomDarkenShader;
	QFXShader*		m_pBloomBlurShader;
	QFXShader*		m_pBloomCombineShader;
	QFXShader*		m_pSSAORenderShader;
	QFXShader*		m_pSSAOBlurShader;
	QFXShader*		m_pSSAOCombineShader;
	QFXShader*		m_pHSLShader;
	QFXShader*		m_pEmbossShader;
	QFXShader*		m_pDOFBlurShader;
	QFXShader*		m_pDOFDepthShader;
	QFXShader*		m_pDOFRenderShader;
	// eezstreet add
	GLuint			m_mbBlurTexture[QFX_MOTIONBLUR_SAMPLES];
	QFXShader*		m_pMotionBlurShader;
	QFXShader*		m_pAnaglyphShader;
	QFXShader*		m_pVignetteShader;
	GLuint			m_uiVignetteTexture;
};

#endif //QFX_RENDERER_H