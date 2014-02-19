#pragma once
#include "Common.h"
#include "VertexBufferObject.h"

class CCatmullRom
{
public:
	CCatmullRom();
	~CCatmullRom();
	glm::vec3 Interpolate(  glm::vec3 &vP0
                          , glm::vec3 &vP1
                          , glm::vec3 &vP2
                          , glm::vec3 &vP3
                          , float t
                          );

    void CreatePath (  glm::vec3 &vP0
                     , glm::vec3 &vP1
                     , glm::vec3 &vP2
                     , glm::vec3 &vP3
                     );

	void RenderPath();
	
private:

	GLuint m_uiVAO;
};


	