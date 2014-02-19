#pragma once

#include "Texture.h"
#include "VertexBufferObjectIndexed.h"

// Class for generating a unit sphere
class CSphere
{
public:
	CSphere();
	~CSphere();
	void Create(string a_sDirectory, string a_sFront, int slicesIn, int stacksIn);
	void Render();
	void Release();
private:
	GLuint m_uiVAO;
	CVertexBufferObjectIndexed m_vboData;
	CTexture m_tTexture;
	string m_sDirectory;
	string m_sFilename;
	int m_iNumTriangles;
};