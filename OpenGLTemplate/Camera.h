#pragma once

#ifdef WIN32
#include "./include/glm/gtc/type_ptr.hpp"
#include "./include/glm/gtc/matrix_transform.hpp"
#endif

#ifdef __APPLE__
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif

class CCamera {
public:
	CCamera();										// Constructor - sets default values for camera position, viewvector, upvector, and speed
	~CCamera();										// Destructor

	glm::vec3 GetPosition() const;					// Gets the position of the camera centre of projection
	glm::vec3 GetView() const;						// Gets the position of the camera view point
	glm::vec3 GetUpVector() const;					// Gets the camera up vector
	glm::vec3 GetStrafeVector() const;				// Gets the camera strafe vector
	glm::mat4* GetPerspectiveProjectionMatrix();	// Gets the camera perspective projection matrix
	glm::mat4* GetOrthographicProjectionMatrix();	// Gets the camera orthographic projection matrix
	glm::mat4 GetViewMatrix();						// Gets the camera view matrix - note this is not stored in the class but returned using glm::lookAt() in GetViewMatrix()

	// Set the camera position, viewpoint, and up vector
	void Set(glm::vec3 &vPosition, glm::vec3 &vViewpoint, glm::vec3 &vUpVector);
	
	// Rotate the camera viewpoint -- this effectively rotates the camera
	void RotateViewPoint(float fAngle, glm::vec3 &vPoint);

	// Respond to mouse movement to rotate the camera
	void SetViewByMouse();

	// Respond to keyboard presses on arrow keys to translate the camera
	void TranslateByKeyboard(double fDt);

	// Strafe the camera (move it side to side)
	void Strafe(double fDirection);

	// Advance the camera (move it forward or backward)
	void Advance(double fDirection);

	// Update the camera
	void Update(double fDt);

	// Set the projection matrices
	void SetPerspectiveProjectionMatrix(float fFOV, float fAspectRatio, float fNear, float fFar);
	void SetOrthographicProjectionMatrix(int iWidth, int iHeight);

	glm::mat3 ComputeNormalMatrix(const glm::mat4 &modelViewMatrix);

private:
	glm::vec3 m_vPosition;			// The position of the camera's centre of projection
	glm::vec3 m_vView;				// The camera's viewpoint (point where the camera is looking)
	glm::vec3 m_vUpVector;			// The camera's up vector
	glm::vec3 m_vStrafeVector;		// The camera's strafe vector

	float m_speed;					// How fast the camera moves

	glm::mat4 m_mPerspectiveProjection;		// Perspective projection matrix
	glm::mat4 m_mOrthographicProjection;	// Orthographic projection matrix
};
