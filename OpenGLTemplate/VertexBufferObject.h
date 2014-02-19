#pragma once

#include "Common.h"

// This class provides a wrapper around an OpenGL Vertex Buffer Object
class CVertexBufferObject
{
public:
	CVertexBufferObject();
	~CVertexBufferObject();

	void Create();									// Creates a VBO
	void Bind();									// Binds the VBO
	void Release();									// Releases the VBO

	void AddData(void* ptrData, UINT uiDataSize);	// Adds data to the VBO
	void UploadDataToGPU(int iUsageHint);			// Uploads the VBO to the GPU

	
private:
	GLuint m_uiVBO;									// VBO id
	vector<BYTE> m_data;							// Data to be put in the VBO
	bool m_bDataUploaded;							// A flag indicating if the data has been sent to the GPU
};