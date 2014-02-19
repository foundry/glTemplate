#include "Common.h"
#include "Plane.h"
#include "Utils.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


CPlane::CPlane()
{}

CPlane::~CPlane()
{}


// Create the plane, including its geometry, texture mapping, normal, and colour
void CPlane::Create(string sDirectory, string sFilename, float fWidth, float fHeight, float fTextureRepeat)
{
	
	m_fwidth = fWidth;
	m_fheight = fHeight;

	// Load the texture
	m_tTexture.Load(sDirectory+sFilename, true);
    glCheckError();
	m_sDirectory = sDirectory;
	m_sFilename = sFilename;

	// Set parameters for texturing using sampler object
	m_tTexture.SetSamplerParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_tTexture.SetSamplerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_tTexture.SetSamplerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_tTexture.SetSamplerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    glCheckError();

	// Use VAO to store state associated with vertices
	glGenVertexArrays(1, &m_uiVAO);
    glCheckError();
	glBindVertexArray(m_uiVAO);
    glCheckError();
	// Create a VBO
	m_vbo.Create();
	m_vbo.Bind();
    glCheckError();
	float fhalfWidth = m_fwidth / 2.0f;
	float fhalfHeight = m_fheight / 2.0f;

	// Vertex positions
	glm::vec3 vPlaneVertices[4] = 
	{
		glm::vec3(-fhalfWidth, 0.0f, -fhalfHeight), 
		glm::vec3(-fhalfWidth, 0.0f, fhalfHeight), 
		glm::vec3(fhalfWidth, 0.0f, -fhalfHeight), 
		glm::vec3(fhalfWidth, 0.0f, fhalfHeight), 
	};

	// Texture coordinates
	glm::vec2 vPlaneTexCoords[4] =
	{
		glm::vec2(0.0f, 0.0f), 
		glm::vec2(0.0f, fTextureRepeat), 
		glm::vec2(fTextureRepeat, 0.0f), 
		glm::vec2(fTextureRepeat, fTextureRepeat)
	};

	// Plane normal
	glm::vec3 vPlaneNormal = glm::vec3(0.0f, 1.0f, 0.0f);

	// Put the vertex attributes in the VBO
	for (unsigned int i = 0; i < 4; i++) {
		m_vbo.AddData(&vPlaneVertices[i], sizeof(glm::vec3));
		m_vbo.AddData(&vPlaneTexCoords[i], sizeof(glm::vec2));
		m_vbo.AddData(&vPlaneNormal, sizeof(glm::vec3));
	}


	// Upload the VBO to the GPU
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);
    glCheckError();
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
    glCheckError();


}

// Render the plane as a triangle strip
void CPlane::Render()
{
    glCheckError();
	glBindVertexArray(m_uiVAO);
    glCheckError();
	m_tTexture.Bind();
    glCheckError();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glCheckError();
}

// Release resources
void CPlane::Release()
{
	m_tTexture.Release();
	glDeleteVertexArrays(1, &m_uiVAO);
	m_vbo.Release();
}