#pragma once

#include "Texture.h"
#include "VertexBufferObject.h"

// This is a class for creating and rendering a skybox
class CSkybox
{
public:
	CSkybox();
	~CSkybox();
	void Create(string sDirectory, string sFront, string sBack, string sLeft, string sRight, string sTop, string sBottom, float fSize);
	void Render();
	void Release();

private:
	GLuint m_uiVAO;
	CVertexBufferObject m_vboData;
	CTexture m_tTextures[6];
	string m_sDirectory;
	string m_sFront, m_sBack, m_sLeft, m_sRight, m_sTop, m_sBottom;
};