#include "renderobject.h"

IRenderObject::IRenderObject()
{
    m_translate = glm::mat4(1.0f);
    m_scale = glm::mat4(1.0f);
    m_rotate = glm::mat4(1.0f);
}

IRenderObject::~IRenderObject()
{

}

glm::mat4 IRenderObject::GetWorld()
{
    return m_world;
}

void IRenderObject::Translate(const glm::mat4& trans)
{
    m_translate = trans;
    //m_world =  m_translate * m_rotate * m_scale;
    m_world = m_translate;
}

void IRenderObject::Rotate(const glm::mat4& rotate)
{
    m_rotate = rotate;
    m_world =  m_translate * m_rotate * m_scale;
}

void IRenderObject::Scale(const glm::mat4& scale)
{
    m_scale = scale;
    m_world =  m_translate * m_rotate * m_scale;
}
