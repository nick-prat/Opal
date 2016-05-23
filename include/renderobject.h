#ifndef _RENDEROBJECT_H
#define _RENDEROBJECT_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include "camera.h"
#include "shader.h"
#include "display.h"

class RenderObject
{
public:
	RenderObject();
	virtual ~RenderObject();
	virtual void Render() = 0;
	virtual void Destroy() = 0;
	
	void Translate(glm::mat4 trans);
	void Rotate(glm::mat4 rotate);
	void Scale(glm::mat4 scale);
	
protected:
	glm::mat4 m_translate;
	glm::mat4 m_scale;
	glm::mat4 m_rotate;
	Display* m_display;
	Camera* m_camera;
	
private:	
	
};

#endif // _RENDEROBJECT_H
