#pragma once

#include "Texture.h"
#include "VertexBufferObject.h"

// Class for generating a xz plane of a given size
class CPlane
{
public:
	CPlane();
	~CPlane();
	void Create(string sDirectory, string sFilename, float fWidth, float fHeight, float fTextureRepeat);
	void Render();
	void Release();
private:
	GLuint m_uiVAO;
	CVertexBufferObject m_vbo;
	CTexture m_tTexture;
	string m_sDirectory;
	string m_sFilename;
	float m_fwidth;
	float m_fheight;
};