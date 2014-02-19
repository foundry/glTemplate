#pragma once

// Class that provides a texture for texture mapping in OpenGL
class CTexture
{
public:
	void CreateFromData(BYTE* bData, int iWidth, int iHeight, int iBPP, GLenum format, bool bGenerateMipMaps = false);
	bool Load(string sPath, bool bGenerateMipMaps = true);
	void Bind(int iTextureUnit = 0);

	void SetSamplerParameter(GLenum parameter, GLenum value);
	void SetSamplerParameterf(GLenum parameter, float value);

	int GetWidth();
	int GetHeight();
	int GetBPP();

	void Release();

	CTexture();
	~CTexture();
private:
	int m_iWidth, m_iHeight, m_iBPP; // Texture width, height, and bytes per pixel
	GLuint m_uiTexture; // Texture name
	GLuint m_uiSampler; // Sampler name
	bool m_bMipMapsGenerated;

	string m_sPath;
};

