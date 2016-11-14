#include "renderobject.hpp"

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
    return m_translate * m_rotate * m_scale;
}

void IRenderObject::Translate(const glm::vec3& trans)
{
    m_translate = glm::translate(m_translate, trans);
}

void IRenderObject::Rotate(const float& degrees, const glm::vec3& rotate)
{
    m_rotate = glm::rotate(m_rotate, degrees, rotate);
}

void IRenderObject::Scale(const glm::vec3& scale)
{
    m_scale = glm::scale(m_scale, scale);
}
