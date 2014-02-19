#include "Common.h"
#include "Utils.h"
#include "texture.h"
#ifdef _WIN32
#include "include\freeimage\FreeImage.h"
#pragma comment(lib, "lib/FreeImage.lib")
#endif
#ifdef __APPLE__
#include <FreeImage.h>
#endif

CTexture::CTexture()
{
	m_bMipMapsGenerated = false;
}
CTexture::~CTexture()
{}

// Create a texture from the data stored in bData.  
void CTexture::CreateFromData(BYTE* bData, int iWidth, int iHeight, int iBPP, GLenum format, bool bGenerateMipMaps)
{
	// Generate an OpenGL texture ID for this texture
	glGenTextures(1, &m_uiTexture);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);
	if(format == GL_RGBA || format == GL_BGRA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	// We must handle this because of internal format parameter
	else if(format == GL_RGB || format == GL_BGR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, iWidth, iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	if(bGenerateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
	glGenSamplers(1, &m_uiSampler);

	m_sPath = "";
	m_bMipMapsGenerated = bGenerateMipMaps;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iBPP = iBPP;
}

// Loads a 2D texture given the filename (sPath).  bGenerateMipMaps will generate a mipmapped texture if true
bool CTexture::Load(string sPath, bool bGenerateMipMaps)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(sPath.c_str(), 0); // Check the file signature and deduce its format

	if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(sPath.c_str());
	
	if(fif == FIF_UNKNOWN) // If still unknown, return failure
		return false;

	if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, sPath.c_str());

    if(!dib) {
        errorMessage("Cannot load image\n%s\n", sPath.c_str());
		return false;
	}

	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data

	// If somehow one of these failed (they shouldn't), return failure
	if(bDataPointer == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0) 
		return false;
	

	GLenum format;
	int bada = FreeImage_GetBPP(dib);
	if(bada == 32)format = GL_BGRA;
	if(bada == 24)format = GL_BGR;
	if(bada == 8) format = GL_LUMINANCE;
	CreateFromData(bDataPointer, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), FreeImage_GetBPP(dib), format, bGenerateMipMaps);
	
	FreeImage_Unload(dib);

	m_sPath = sPath;

	return true; // Success
}

void CTexture::SetSamplerParameter(GLenum parameter, GLenum value)
{
	glSamplerParameteri(m_uiSampler, parameter, value);
}

void CTexture::SetSamplerParameterf(GLenum parameter, float value)
{
	glSamplerParameterf(m_uiSampler, parameter, value);
}


// Binds a texture for rendering
void CTexture::Bind(int iTextureUnit)
{
	glActiveTexture(GL_TEXTURE0+iTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);
	glBindSampler(iTextureUnit, m_uiSampler);
}

// Frees memory on the GPU of the texture
void CTexture::Release()
{
	glDeleteSamplers(1, &m_uiSampler);
	glDeleteTextures(1, &m_uiTexture);
}

int CTexture::GetWidth()
{
	return m_iWidth;
}

int CTexture::GetHeight()
{
	return m_iHeight;
}

int CTexture::GetBPP()
{
	return m_iBPP;
}