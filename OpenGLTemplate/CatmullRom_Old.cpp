#include "CatmullRom_Old.h"
#include "Utils.h"



CCatmullRom::CCatmullRom(){}
CCatmullRom::~CCatmullRom(){}


glm::vec3  CCatmullRom::Interpolate(  glm::vec3 &vP0
                                    , glm::vec3 &vP1
                                    , glm::vec3 &vP2
                                    , glm::vec3 &vP3
                                    , float t)
{
    glm::vec3 a = vP1;
    glm::vec3 b = 0.5f * ( -vP0 + vP2 );
    glm::vec3 c = 0.5f * (2.0f*vP0 - 5.0f*vP1 + 4.0f*vP2 - vP3);
    glm::vec3 d = 0.5f * (-vP0 + 3.0f*vP1 - 3.0f * vP2 + vP3);


    return a + b*t + c*t*t + d*t*t*t;

}

void CCatmullRom::RenderPath() {
    glCheckError();
    glBindVertexArray(m_uiVAO);
    glCheckError();
    glEnable(GL_LINE_SMOOTH);
    glCheckError();
    GLfloat lineWidth = 2.0f;
    glLineWidth(lineWidth);
    glCheckError();
    glDisable(GL_LINE_SMOOTH);
    glCheckError();
    glDrawArrays(GL_LINE_STRIP, 0, 100);
    glCheckError();
}

void CCatmullRom::CreatePath( glm::vec3 &vP0
                             , glm::vec3 &vP1
                             , glm::vec3 &vP2
                             , glm::vec3 &vP3
                             )

{
//use VAO to store state associated with vertices
    glGenVertexArrays(1, &m_uiVAO);
    glBindVertexArray(m_uiVAO);

    //create a VBO
    CVertexBufferObject vboData;
    vboData.Create();
    vboData.Bind();

    glm::vec2 vTexCoord(0.0f, 0.0f);
    glm::vec3 vNormal(0.0f, 1.0f, 0.0f);
    for (unsigned int i = 0; i< 100; i++) {
        float t = (float) i / 100.0f;
        glm::vec3 v = Interpolate(vP0, vP1, vP2, vP3, t);
        vboData.AddData(&v,sizeof(glm::vec3));
        vboData.AddData(&vTexCoord, sizeof(glm::vec2));
        vboData.AddData(&vNormal, sizeof(glm::vec3));

    }

    //Upload the VBO to the GPU
    vboData.UploadDataToGPU(GL_STATIC_DRAW);

    //set the vertex attribute locations
    GLsizei istride = 2 * sizeof(glm::vec3)+sizeof(glm::vec2);

    //Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);
    //Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, istride, (void*)sizeof(glm::vec3));

    //Normal vectores
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, istride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));


}

