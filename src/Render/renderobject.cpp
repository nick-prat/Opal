#include "renderobject.hpp"

IRenderObject::IRenderObject()
{
    m_world = glm::mat4(1.0f);
}

IRenderObject::~IRenderObject()
{

}

glm::mat4 IRenderObject::GetWorld()
{
    return m_world;
}
