#pragma once

#ifdef WIN32
#include <ft2build.h>
#include FT_FREETYPE_H
#endif
#ifdef __APPLE__
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#endif

#include "Common.h"
#include "Texture.h"
#include "Shaders.h"
#include "VertexBufferObject.h"


// This class is a wrapper for FreeType fonts and their usage with OpenGL
class CFreeTypeFont
{
public:
	CFreeTypeFont();
	~CFreeTypeFont();

	bool LoadFont(string sFile, int iPXSize);
	bool LoadSystemFont(string sName, int iPXSize);

	int GetTextWidth(string sText, int iPXSize);

	void Print(string sText, int x, int y, int iPXSize = -1);
	void Render(int x, int y, int iPXSize, char* sText, ...);

	void ReleaseFont();

	void SetShaderProgram(CShaderProgram* a_shShaderProgram);

private:
	void CreateChar(int iIndex);

	CTexture m_tCharTextures[256];
	int m_iAdvX[256], m_iAdvY[256];
	int m_iBearingX[256], m_iBearingY[256];
	int m_iCharWidth[256], m_iCharHeight[256];
	int m_iLoadedPixelSize, m_iNewLine;

	bool m_bLoaded;

	GLuint m_uiVAO;
	CVertexBufferObject m_vboData;

	FT_Library m_ftLib;
	FT_Face m_ftFace;
	CShaderProgram* m_shShaderProgram;
};
