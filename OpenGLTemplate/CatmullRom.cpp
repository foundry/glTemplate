#include "CatmullRom.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Utils.h"



CCatmullRom::CCatmullRom()
{
	m_indexCount = 0;
	m_K = 500; //points per circle
    m_fCurrentDistance = 0.0f;
    m_numberOfSamples = 2000;
    m_radius = 15.0f;
}

CCatmullRom::~CCatmullRom()
{}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between vP1 and vP2
glm::vec3 CCatmullRom::Interpolate(glm::vec3 &vP0, glm::vec3 &vP1, glm::vec3 &vP2, glm::vec3 &vP3, float t)
{
    /*
    printf("Interpolate\n");
    logVec3(vP0);
    logVec3(vP1);

    logVec3(vP2);

    logVec3(vP3);
    printf("\n");
*/

    float t2 = t * t;
    float t3 = t2 * t;

	glm::vec3 a = vP1;
	glm::vec3 b = 0.5f * (-vP0 + vP2);
	glm::vec3 c = 0.5f * (2.0f*vP0 - 5.0f*vP1 + 4.0f*vP2 - vP3);
	glm::vec3 d = 0.5f * (-vP0 + 3.0f*vP1 - 3.0f*vP2 + vP3);

	return a + b*t + c*t2 + d*t3;

}


void CCatmullRom::SetControlPoints()
{
    SetControlPointsFigureOfEight2();
}

void CCatmullRom::SetControlPointsFigureOfEight2() {
    m_controlPoints.push_back(glm::vec3( 000, 35,  000));
    m_controlPoints.push_back(glm::vec3( 100, 45,  100));
    m_controlPoints.push_back(glm::vec3( 100, 55,  200));
    m_controlPoints.push_back(glm::vec3( 000, 65,  300));
    m_controlPoints.push_back(glm::vec3(-100, 75,  200));
    m_controlPoints.push_back(glm::vec3( 000,  75,  000));
    m_controlPoints.push_back(glm::vec3( 100,  65, -100));
    m_controlPoints.push_back(glm::vec3( 100,  55, -200));
    m_controlPoints.push_back(glm::vec3( 000,  45, -300));
    m_controlPoints.push_back(glm::vec3(-100,  35, -200));
    m_controlPoints.push_back(glm::vec3(-100,  35, -100));
}



void CCatmullRom::SetControlPointsFigureOfEight() {
    m_controlPoints.push_back(glm::vec3( 000, 35,  000));
    m_controlPoints.push_back(glm::vec3( 100, 35,  100));
    m_controlPoints.push_back(glm::vec3( 100, 35,  200));
    m_controlPoints.push_back(glm::vec3( 000, 35,  300));
    m_controlPoints.push_back(glm::vec3(-100, 35,  200));
    m_controlPoints.push_back(glm::vec3( 000,  35,  000));
    m_controlPoints.push_back(glm::vec3( 100,  35, -100));
    m_controlPoints.push_back(glm::vec3( 100,  35, -200));
    m_controlPoints.push_back(glm::vec3( 000,  35, -300));
    m_controlPoints.push_back(glm::vec3(-100,  35, -200));
    m_controlPoints.push_back(glm::vec3(-100,  35, -100));
}

void CCatmullRom::SetControlPointsOval() {
    m_controlPoints.push_back(glm::vec3(100, 35, 0));
    m_controlPoints.push_back(glm::vec3(100, 35, 200));
    m_controlPoints.push_back(glm::vec3(71, 35, 271));
    m_controlPoints.push_back(glm::vec3(0, 35, 300));
    m_controlPoints.push_back(glm::vec3(-71, 35, 271));
    m_controlPoints.push_back(glm::vec3(-100, 35, 200));
    m_controlPoints.push_back(glm::vec3(-100, 35, 0));
    m_controlPoints.push_back(glm::vec3(-100, 35, -200));
    m_controlPoints.push_back(glm::vec3(-71, 35, -271));
    m_controlPoints.push_back(glm::vec3(0, 35, -300));
    m_controlPoints.push_back(glm::vec3(71, 35, -271));
    m_controlPoints.push_back(glm::vec3(100, 35, -200));
    m_controlPoints.push_back(glm::vec3(100, 35, 0));

}

void CCatmullRom::SetControlPointsLine() {
    m_controlPoints.push_back(glm::vec3(100, 35, 0));
    m_controlPoints.push_back(glm::vec3(90, 35, 0));
    m_controlPoints.push_back(glm::vec3(80, 35, 0));
    m_controlPoints.push_back(glm::vec3(70, 35, 0));
    m_controlPoints.push_back(glm::vec3(60, 35, 0));
    m_controlPoints.push_back(glm::vec3(50, 35, 0));
    m_controlPoints.push_back(glm::vec3(40, 35, 0));
    m_controlPoints.push_back(glm::vec3(30, 35, 0));
    m_controlPoints.push_back(glm::vec3(20, 35, 0));
    m_controlPoints.push_back(glm::vec3(10, 35, 0));
    m_controlPoints.push_back(glm::vec3(0, 35, 0));

}


// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::ComputeLengthsAlongControlPoints()
{
	int M = (int) m_controlPoints.size();

	float fAccumulatedLength = 0.0f;
	m_fvDistances.push_back(fAccumulatedLength);
	for (int i = 1; i < M; i++) {
		fAccumulatedLength += glm::distance(m_controlPoints[i-1], m_controlPoints[i]);
		m_fvDistances.push_back(fAccumulatedLength);
	}

	// Get the distance from the last point to the first
	fAccumulatedLength += glm::distance(m_controlPoints[M-1], m_controlPoints[0]);
	m_fvDistances.push_back(fAccumulatedLength);
}


// Return the point based on a distance d along the control polygon
glm::vec3 CCatmullRom::PointAt(float d)
{
	if (d < 0)
		return glm::vec3(0, 0, 0); 		// Sorry, only taking positive values of d!

	int M = (int) m_controlPoints.size();
	if (M == 0)
		return glm::vec3(0, 0, 0); 		// No points available - must call UniformlySampleControlPoints first!


	float fTotalLength = m_fvDistances[m_fvDistances.size()-1];

	// The the current length along the control polygon; handle the case where we've looped around the track
	float fLength = d - (int) (d / fTotalLength) * fTotalLength;

	// Find the current segment
	int j = -1;
	for (int i = 0; i < (int) m_fvDistances.size(); i++) {
		if (fLength >= m_fvDistances[i] && fLength < m_fvDistances[i+1]) {
			j = i; // found it!
			break;
		}
	}

	if (j == -1)
		return glm::vec3(0, 0, 0);  // Didn't find a valid segment -- shouldn't be here

	// Interpolate on current segment -- get t
	float fSegmentLength =  m_fvDistances[j+1] - m_fvDistances[j];	
	float t = (fLength - m_fvDistances[j]) / fSegmentLength;	
	
	// Get the indices of the four points along the control polygon for the current segment
	int iPrev = ((j-1) + M) % M;
	int iCur = j;
	int iNext = (j + 1) % M;
	int iNextNext = (j + 2) % M;

	// Interpolate to get the point
	return Interpolate(m_controlPoints[iPrev], m_controlPoints[iCur], m_controlPoints[iNext], m_controlPoints[iNextNext], t);
}



// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::UniformlySampleControlPoints(int iNumSamples)
{
	// Compute the lengths of each segment along the control polygon, and the total length
	ComputeLengthsAlongControlPoints();
	float fTotalLength = m_fvDistances[m_fvDistances.size()-1];

	// The spacing will be based on the control polygon
	float fSpacing = fTotalLength / iNumSamples;

	// Call PointAt to sample the spline, to generate the points
	for (int i = 0; i < iNumSamples; i++) 
		m_centrelinePoints.push_back(PointAt(i * fSpacing));

}



void CCatmullRom::CreateCentreline()
{
	// Call Set Control Points
    SetControlPoints();


	// Call UniformlySampleControlPoints with the number of samples required
    UniformlySampleControlPoints(m_numberOfSamples);

	// Create a VAO called m_uiVAOCentreline and a VBO to get the points onto the graphics card
    glGenVertexArrays(1, &m_uiVAOCentreline);
    glBindVertexArray(m_uiVAOCentreline);

    //create a VBO
    CVertexBufferObject vboData;
    vboData.Create();
    vboData.Bind();


    glm::vec2 vTexCoord(0.0f, 0.0f);
    glm::vec3 vNormal(0.0f, 1.0f, 0.0f);
    size_t numberOfPoints = m_centrelinePoints.size();
    for (unsigned int i = 0; i< numberOfPoints; i++) {
        glm::vec3 v = m_centrelinePoints.at(i);
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


void CCatmullRom::CreateCircles()
{
	unsigned int K = m_K;

    

	// Generate a VAO called m_uiVAOCircles and a VBO to get the circle points on the graphics card


    glGenVertexArrays(1, &m_uiVAOCircles);
    glBindVertexArray(m_uiVAOCircles);

    //create a VBO
    CVertexBufferObject vboData;
    vboData.Create();
    vboData.Bind();


	// Compute the circles, one for each point on the centreline.

    size_t numberOfCircles = m_centrelinePoints.size();
    for (GLuint i = 0; i< numberOfCircles-1; i++) {
        //determine pNext, the next point on the centerline
        glm::vec3 thisPoint  = m_centrelinePoints.at(i);
        //printf("\nc thisPoint\n");
        //logVec3(thisPoint);
        glm::vec3 nextPoint = m_centrelinePoints.at(i+1);


        glm::vec3 upVector  = glm::vec3(0,1,0);
        glm::vec3 tangent = nextPoint - thisPoint;
        glm::vec3 nTangent = glm::normalize(tangent);
        glm::vec3 nNormal = glm::normalize(glm::cross(nTangent, upVector));
        glm::vec3 nBinormal = glm::normalize(glm::cross(nNormal,nTangent));

        for (GLuint k = 0; k <= K; k++) {
            GLfloat theta = 2*M_PI*k/K;
            glm::vec3 c = thisPoint + nNormal * m_radius * cos(theta) + nBinormal * m_radius * sin(theta);
            m_circlePoints.push_back(c);
            vboData.AddData(&c,sizeof(glm::vec3));
        }
    }

    vboData.UploadDataToGPU(GL_STATIC_DRAW);
    GLsizei istride = sizeof(glm::vec3);
    //Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);

}


void CCatmullRom::CreateTube()
{
	
	unsigned int K = m_K;



	// Generate a VAO called m_uiVAOTube and an indexed VBO to get the circle points and tube indices on the graphics card

    glGenVertexArrays(1, &m_uiVAOTube);
    glBindVertexArray(m_uiVAOTube);
    CVertexBufferObjectIndexed vboiData;
    vboiData.Create();
    vboiData.Bind();

	// Compute the indices to connect the triangles
    m_numberOfTriangles = 0;
    size_t numberOfCircles = m_centrelinePoints.size();
    for (GLuint i = 0; i< numberOfCircles-3; i++) {
    //for point i on the centerline
        //determine the starting index s for the curent circle s=i*(K+1)
        GLuint s=i*(K+1);
        for (GLuint k = 0; k <= K; k++) {

        //for each point k 0-K on the current circle


            std::vector<glm::vec3> vertices;
            std::vector<GLuint> indices;
            glm::vec3 circlePoint;

            circlePoint = m_circlePoints.at(s+k);
            vertices.push_back(circlePoint);

            circlePoint = m_circlePoints.at(s+k+1);
            vertices.push_back(circlePoint);

            circlePoint = m_circlePoints.at(s+k+K+1);
            vertices.push_back(circlePoint);

            circlePoint = m_circlePoints.at(s+k+K+2);
            vertices.push_back(circlePoint);


            //triangle 1
            indices.push_back(s+k);
            indices.push_back(s+k+K+2);
            indices.push_back(s+k+K+1);

            //triangle2
            indices.push_back(s+k);
            indices.push_back(s+k+1);
            indices.push_back(s+k+K+2);
            m_indexCount += 6;
  

            vboiData.AddVertexData(&vertices, sizeof(vertices));
            vboiData.AddIndexData(&indices, sizeof(indices));
            m_numberOfTriangles++;
            m_numberOfTriangles++;

            //form a first triangle using vertex indices s+k, s+k+2+K,s+k+1+K
            //form a second triangle using vetex indices s+k, s+k_1_K. s+k+2+K
            //store the six indices in the order given above
        }


    }
    vboiData.UploadDataToGPU(GL_STATIC_DRAW);

	GLsizei stride = 3*sizeof(glm::vec3);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

}


void CCatmullRom::RenderCentreline()
{
	glBindVertexArray(m_uiVAOCentreline);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glDrawArrays(GL_LINE_STRIP, 0, m_numberOfSamples);

	// Write the code to render the centreline
}

void CCatmullRom::RenderCircles()
{
	glBindVertexArray(m_uiVAOCircles);
    glCheckError();


    glPointSize(1.0f);
    glCheckError();

    glDrawArrays(GL_LINES, 0, (GLsizei)m_circlePoints.size());
    glCheckError();


	// Write the code to render the circle points
}

void CCatmullRom::RenderTube()
{
	glBindVertexArray(m_uiVAOTube);

    glCheckError();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    //glDisable(GL_CULL_FACE);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT,0);

    glCheckError();

	// Write the code to render the tube
		
}
