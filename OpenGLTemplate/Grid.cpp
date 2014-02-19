#include "Common.h"

#include "Grid.h"

CGrid::CGrid()
{
	m_iVertices = 0;
}

CGrid::~CGrid()
{}


// Create a planar grid
void CGrid::Create(float fWidth, float fHeight, int iLines)
{
	
	if (iLines < 2)
		return;

	// Use VAO to store state associated with vertices
	glGenVertexArrays(1, &m_uiVAO);
	glBindVertexArray(m_uiVAO);

	// Create a VBO
	m_vboRenderData.Create();
	m_vboRenderData.Bind();

	glm::vec2 vTex = glm::vec2(0, 0);
	glm::vec3 vNormal = glm::vec3(0, 1, 0);

	
	// Add lines along x
	for (int i = 0; i < iLines; i++) {
		float t = ((float) i / (float) (iLines-1));
		glm::vec3 p1 = glm::vec3(-fWidth / 2, 0, -fHeight / 2 + t * fHeight);
		m_vboRenderData.AddData(&p1, sizeof(glm::vec3));
		m_vboRenderData.AddData(&vTex, sizeof(glm::vec2));
		m_vboRenderData.AddData(&vNormal, sizeof(glm::vec3));

		glm::vec3 p2 = glm::vec3(fWidth / 2, 0, -fHeight / 2 + t * fHeight);
		m_vboRenderData.AddData(&p2, sizeof(glm::vec3));
		m_vboRenderData.AddData(&vTex, sizeof(glm::vec2));
		m_vboRenderData.AddData(&vNormal, sizeof(glm::vec3));
		
		m_iVertices += 2;
	}
	
	
	
	// Add lines along z
	for (int i = 0; i < iLines; i++) {
		float t = ((float) i / (float) (iLines-1));
		glm::vec3 p1 = glm::vec3(-fWidth / 2 + t * fWidth, 0, -fHeight / 2);
		m_vboRenderData.AddData(&p1, sizeof(glm::vec3));
		m_vboRenderData.AddData(&vTex, sizeof(glm::vec2));
		m_vboRenderData.AddData(&vNormal, sizeof(glm::vec3));

		glm::vec3 p2 = glm::vec3(-fWidth / 2 + t * fWidth, 0, fHeight / 2);
		m_vboRenderData.AddData(&p2, sizeof(glm::vec3));
		m_vboRenderData.AddData(&vTex, sizeof(glm::vec2));
		m_vboRenderData.AddData(&vNormal, sizeof(glm::vec3));

		m_iVertices += 2;

	}
	


	// Upload the VBO to the GPU
	m_vboRenderData.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations
	GLsizei istride = 2*sizeof(glm::vec3)+sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, istride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, istride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));
	
}

// Render the grid with lines
void CGrid::Render()
{
	if (m_iVertices < 2)
		return;

	glLineWidth(1.0);
	glBindVertexArray(m_uiVAO);
	glDrawArrays(GL_LINES, 0, m_iVertices);
}

// Release resources
void CGrid::Release()
{
	glDeleteVertexArrays(1, &m_uiVAO);
	m_vboRenderData.Release();
}