#pragma once

#include "VertexBufferObject.h"

// Class for rendering a grid-like terrain
class CGrid
{
public:
	CGrid();
	~CGrid();
	void Create(float fWidth, float fHeight, int iLines);
	void Render();
	void Release();
private:
	GLuint m_uiVAO;
	CVertexBufferObject m_vboRenderData;
	glm::vec4 m_vColour;	
	int m_iVertices;
};