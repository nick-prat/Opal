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
	m_world =  m_translate * m_rotate * m_scale;
}

void RenderObject::Rotate(glm::mat4 rotate)
{
	m_rotate = rotate;
	m_world =  m_translate * m_rotate * m_scale;
}

void RenderObject::Scale(glm::mat4 scale)
{
	m_scale = scale;
	m_world =  m_translate * m_rotate * m_scale;
}