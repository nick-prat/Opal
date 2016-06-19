#ifndef _RENDEROBJECT_H
#define _RENDEROBJECT_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "Model/Shader/shader.h"
#include "Display/display.h"

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
	
	glm::mat4 GetWorld();
	
protected:
	std::shared_ptr<Display> m_display;
	
private:	
	glm::mat4 m_translate;
	glm::mat4 m_scale;
	glm::mat4 m_rotate;
	glm::mat4 m_world;
	
};

#endif // _RENDEROBJECT_H
