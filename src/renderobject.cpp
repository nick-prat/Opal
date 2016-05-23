#include "renderobject.h"

RenderObject::RenderObject()
{
	m_translate = glm::mat4(1.0f);
	m_scale = glm::mat4(1.0f);
	m_rotate = glm::mat4(1.0f);
}

RenderObject::~RenderObject()
{
	
}

void RenderObject::Translate(glm::mat4 trans)
{
	m_translate = trans;
}

void RenderObject::Rotate(glm::mat4 rotate)
{
	m_rotate = rotate;
}

void RenderObject::Scale(glm::mat4 scale)
{
	m_scale = scale;
}