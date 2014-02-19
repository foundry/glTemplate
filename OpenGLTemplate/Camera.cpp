#include "camera.h"

#ifdef WIN32
#include "gamewindow.h"
#endif

// Constructor for camera -- initialise with some default values
CCamera::CCamera()
{
	m_vPosition = glm::vec3(0.0f, 10.0f, 100.0f);
	m_vView = glm::vec3(0.0f, 0.0f, 0.0f);
	m_vUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
	m_speed = 0.025f;
}
CCamera::~CCamera()
{}
 
// Set the camera at a specific position, looking at the view point, with a given up vector
void CCamera::Set(glm::vec3 &vPosition, glm::vec3 &vViewpoint, glm::vec3 &vUpVector)
{
	m_vPosition = vPosition;
	m_vView = vViewpoint;
	m_vUpVector = vUpVector;

}

#ifdef WIN32
// Respond to mouse movement
void CCamera::SetViewByMouse()
{  
	int iMiddle_x = GameWindow::SCREEN_WIDTH >> 1;
	int iMiddle_y = GameWindow::SCREEN_HEIGHT >> 1;

	float fAngle_y = 0.0f;
	float fAngle_z = 0.0f;
	static float fRotation_x = 0.0f;

	POINT mouse;
	GetCursorPos(&mouse);

	if (mouse.x == iMiddle_x && mouse.y == iMiddle_y) {
		return;
	}

	SetCursorPos(iMiddle_x, iMiddle_y);

	fAngle_y = (float) (iMiddle_x - mouse.x) / 1000.0f;
	fAngle_z = (float) (iMiddle_y - mouse.y) / 1000.0f;

	fRotation_x -= fAngle_z;

	float fMaxAngle = 1.56f; // Just a little bit below PI / 2

	if (fRotation_x > fMaxAngle) {
		fRotation_x = fMaxAngle;
	} else if (fRotation_x < -fMaxAngle) {
		fRotation_x = -fMaxAngle;
	} else {
		glm::vec3 cross = glm::cross(m_vView - m_vPosition, m_vUpVector);
		glm::vec3 axis = glm::normalize(cross);

		RotateViewPoint(fAngle_z, axis);
	}

	RotateViewPoint(fAngle_y, glm::vec3(0, 1, 0));
}
#endif

// Rotate the camera view point -- this effectively rotates the camera since it is looking at the view point
void CCamera::RotateViewPoint(float fAngle, glm::vec3 &vPoint)
{
	glm::vec3 vView = m_vView - m_vPosition;
	
	glm::mat4 R = glm::rotate(glm::mat4(1), fAngle * 180.0f / (float) M_PI, vPoint);
	glm::vec4 newView = R * glm::vec4(vView, 1);

	m_vView = m_vPosition + glm::vec3(newView);
}

// Strafe the camera (side to side motion)
void CCamera::Strafe(double fDirection)
{
	float fSpeed = (float) (m_speed * fDirection);

	m_vPosition.x = m_vPosition.x + m_vStrafeVector.x * fSpeed;
	m_vPosition.z = m_vPosition.z + m_vStrafeVector.z * fSpeed;

	m_vView.x = m_vView.x + m_vStrafeVector.x * fSpeed;
	m_vView.z = m_vView.z + m_vStrafeVector.z * fSpeed;
}

// Advance the camera (forward / backward motion)
void CCamera::Advance(double fDirection)
{
	float fSpeed = (float) (m_speed * fDirection);

	glm::vec3 vView = glm::normalize(m_vView - m_vPosition);
	m_vPosition = m_vPosition + vView * fSpeed;
	m_vView = m_vView + vView * fSpeed;

}

// Update the camera to respond to mouse motion for rotations and keyboard for translation
void CCamera::Update(double fDt)
{
	glm::vec3 vCross = glm::cross(m_vView - m_vPosition, m_vUpVector);
	m_vStrafeVector = glm::normalize(vCross);
#ifdef WIN32
	SetViewByMouse();
    TranslateByKeyboard(fDt);
#endif
}

#ifdef WIN32

// Update the camera to respond to key presses for translation
void CCamera::TranslateByKeyboard(double fDt)
{
	if(GetKeyState(VK_UP) & 0x80) {		
		Advance(1.0*fDt);	
	}

	if(GetKeyState(VK_DOWN) & 0x80) {	
		Advance(-1.0*fDt);		
	}

	if(GetKeyState(VK_LEFT) & 0x80) {	
		Strafe(-1.0*fDt);
	}

	if(GetKeyState(VK_RIGHT) & 0x80 ) {			
		Strafe(1.0*fDt);
	}	
}
#endif

// Return the camera position
glm::vec3 CCamera::GetPosition() const
{
	return m_vPosition;
}

// Return the camera view point
glm::vec3 CCamera::GetView() const
{
	return m_vView;
}

// Return the camera up vector
glm::vec3 CCamera::GetUpVector() const
{
	return m_vUpVector;
}

// Return the camera strafe vector
glm::vec3 CCamera::GetStrafeVector() const
{
	return m_vStrafeVector;
}

// Return the camera perspective projection matrix
glm::mat4* CCamera::GetPerspectiveProjectionMatrix()
{
	return &m_mPerspectiveProjection;
}

// Return the camera orthographic projection matrix
glm::mat4* CCamera::GetOrthographicProjectionMatrix()
{
	return &m_mOrthographicProjection;
}

// Set the camera perspective projection matrix to produce a view frustum with a specific field of view, aspect ratio, 
// and near / far clipping planes
void CCamera::SetPerspectiveProjectionMatrix(float fFOV, float fAspectRatio, float fNear, float fFar)
{
	m_mPerspectiveProjection = glm::perspective(fFOV, fAspectRatio, fNear, fFar);
}

// The the camera orthographic projection matrix to match the width and height passed in
void CCamera::SetOrthographicProjectionMatrix(int iWidth, int iHeight)
{
	m_mOrthographicProjection = glm::ortho(0.0f, float(iWidth), 0.0f, float(iHeight));
}

// Get the camera view matrix
glm::mat4 CCamera::GetViewMatrix()
{
	return glm::lookAt(m_vPosition, m_vView, m_vUpVector);
}

// The normal matrix is used to transform normals to eye coordinates -- part of lighting calculations
glm::mat3 CCamera::ComputeNormalMatrix(const glm::mat4 &modelViewMatrix)
{
	return glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));
}

