#include "VertexBufferObject.h"


// Constructor -- initialise member variable m_bDataUploaded to false
CVertexBufferObject::CVertexBufferObject()
{
	m_bDataUploaded = false;
}

CVertexBufferObject::~CVertexBufferObject()
{
}

// Create a VBO 
void CVertexBufferObject::Create()
{
	glGenBuffers(1, &m_uiVBO);
}

// Release the VBO and any associated data
void CVertexBufferObject::Release()
{
	glDeleteBuffers(1, &m_uiVBO);
	m_bDataUploaded = false;
	m_data.clear();
}


// Binds a VBO.  
void CVertexBufferObject::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
}


// Uploads the data to the GPU.  Afterwards, the data can be cleared.  
// iUsageHint - GL_STATIC_DRAW, GL_DYNAMIC_DRAW...
void CVertexBufferObject::UploadDataToGPU(int iDrawingHint)
{
	glBufferData(GL_ARRAY_BUFFER, m_data.size(), &m_data[0], iDrawingHint);
	m_bDataUploaded = true;
	m_data.clear();
}

// Adds data to the VBO.  
void CVertexBufferObject::AddData(void* ptrData, UINT uiDataSize)
{
	m_data.insert(m_data.end(), (BYTE*)ptrData, (BYTE*)ptrData+uiDataSize);
}



