#pragma once
#include "Common.h"
#include "vertexBufferObject.h"
#include "vertexBufferObjectIndexed.h"
#include "Texture.h"


class CCatmullRom
{
public:
	CCatmullRom();
	~CCatmullRom();

	void CreateCentreline();
	void RenderCentreline();

	void CreateCircles();
	void RenderCircles();

	void CreateTube();
	void RenderTube();

    float m_fCurrentDistance;
	glm::vec3 PointAt(float d);
	glm::vec3 Interpolate(glm::vec3 &vP0, glm::vec3 &vP1, glm::vec3 &vP2, glm::vec3 &vP3, float t);


private:

	void SetControlPoints();
	void ComputeLengthsAlongControlPoints();
	void UniformlySampleControlPoints(int iNumSamples);


	vector<float> m_fvDistances;
	CTexture m_tTexture;

	GLuint m_uiVAOCentreline;
	GLuint m_uiVAOCircles;
	GLuint m_uiVAOTube;

    GLuint m_numberOfSamples;
    GLuint m_numberOfTriangles;

	vector<glm::vec3> m_controlPoints;

	vector<glm::vec3> m_centrelinePoints;

	vector<glm::vec3> m_circlePoints;
	vector<glm::vec3> m_circleNormals;
	vector<glm::vec2> m_circleTextureCoords;

	unsigned int m_indexCount;
	unsigned int m_K;
    float m_radius;

    void SetControlPointsFigureOfEight2();
    void SetControlPointsFigureOfEight();
    void SetControlPointsOval();
    void SetControlPointsLine();



};
