#include "camera.h"

Camera::Camera()
{
	m_viewMatrix = glm::mat4(1.0f);
}

Camera::~Camera()
{
	
}
	
bool Camera::InitCamera()
{
	m_viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
		
	return true;
}

void Camera::Destroy()
{
	
}
	
glm::mat4 Camera::GetViewMatrix()
{
	//glm::mat4 view = m_viewMatrix * 
	
	return m_viewMatrix;
}

void Camera::RotateCamera(glm::mat4 rotation)
{
	
}

void Camera::MoveCamera(glm::vec3 delta)
{
	
}