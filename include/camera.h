#ifndef _CAMERA_H
#define _CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

class Camera
{
public:
	
	Camera();
	~Camera();
	
	bool InitCamera();
	void Destroy();
	
	glm::mat4 GetViewMatrix();
	void MoveCamera(glm::vec3 delta);
	void RotateCamera(glm::mat4 rotation);
	
protected:
	
private:
	glm::mat4 m_viewMatrix;
};

#endif