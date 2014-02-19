#include "Common.h"

#include "skybox.h"


CSkybox::CSkybox()
{}

CSkybox::~CSkybox()
{}


// Create a skybox of a given size with six textures
void CSkybox::Create(string sDirectory, string sFront, string sBack, string sLeft, string sRight, string sTop, string sBottom, float fSize)
{
	m_tTextures[0].Load(sDirectory + sFront);
	m_tTextures[1].Load(sDirectory + sBack);
	m_tTextures[2].Load(sDirectory + sLeft);
	m_tTextures[3].Load(sDirectory + sRight);
	m_tTextures[4].Load(sDirectory + sTop);
	m_tTextures[5].Load(sDirectory + sBottom);

	m_sDirectory = sDirectory;

	m_sFront = sFront;
	m_sBack = sBack;
	m_sLeft = sLeft;
	m_sRight = sRight;
	m_sTop = sTop;
	m_sBottom = sBottom;
	 
	for (int i = 0; i < 6; i++) {
		m_tTextures[i].SetSamplerParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_tTextures[i].SetSamplerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_tTextures[i].SetSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_tTextures[i].SetSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glGenVertexArrays(1, &m_uiVAO);
	glBindVertexArray(m_uiVAO);

	m_vboData.Create();
	m_vboData.Bind();


	glm::vec3 vSkyBoxVertices[24] = 
	{
		// Front face
		glm::vec3(fSize, fSize, fSize), glm::vec3(fSize, -fSize, fSize), glm::vec3(-fSize, fSize, fSize), glm::vec3(-fSize, -fSize, fSize),
		// Back face
		glm::vec3(-fSize, fSize, -fSize), glm::vec3(-fSize, -fSize, -fSize), glm::vec3(fSize, fSize, -fSize), glm::vec3(fSize, -fSize, -fSize),
		// Left face
		glm::vec3(-fSize, fSize, fSize), glm::vec3(-fSize, -fSize, fSize), glm::vec3(-fSize, fSize, -fSize), glm::vec3(-fSize, -fSize, -fSize),
		// Right face
		glm::vec3(fSize, fSize, -fSize), glm::vec3(fSize, -fSize, -fSize), glm::vec3(fSize, fSize, fSize), glm::vec3(fSize, -fSize, fSize),
		// Top face
		glm::vec3(-fSize, fSize, -fSize), glm::vec3(fSize, fSize, -fSize), glm::vec3(-fSize, fSize, fSize), glm::vec3(fSize, fSize, fSize),
		// Bottom face
		glm::vec3(fSize, -fSize, -fSize), glm::vec3(-fSize, -fSize, -fSize), glm::vec3(fSize, -fSize, fSize), glm::vec3(-fSize, -fSize, fSize),
	};
	glm::vec2 vSkyBoxTexCoords[4] =
	{
		glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)
	};

	glm::vec3 vSkyBoxNormals[6] = 
	{
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};

	glm::vec4 vColour = glm::vec4(1, 1, 1, 1);
	for (int i = 0; i < 24; i++) {
		m_vboData.AddData(&vSkyBoxVertices[i], sizeof(glm::vec3));
		m_vboData.AddData(&vSkyBoxTexCoords[i%4], sizeof(glm::vec2));
		m_vboData.AddData(&vSkyBoxNormals[i/4], sizeof(glm::vec3));
	}

	m_vboData.UploadDataToGPU(GL_STATIC_DRAW);

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

// Render the skybox
void CSkybox::Render()
{
	glDepthMask(0);
	glBindVertexArray(m_uiVAO);
	for (int i = 0; i < 6; i++) {
		m_tTextures[i].Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
	}
	glDepthMask(1);
}

// Release the storage assocaited with the skybox
void CSkybox::Release()
{
	for (int i = 0; i < 6; i++)
		m_tTextures[i].Release();
	glDeleteVertexArrays(1, &m_uiVAO);
	m_vboData.Release();
}