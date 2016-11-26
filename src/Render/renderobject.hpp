#ifndef _RENDEROBJECT_H
#define _RENDEROBJECT_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include <Render/Shader/shader.hpp>
#include <Core/display.hpp>

class IRenderObject
{
public:
    IRenderObject();
    virtual ~IRenderObject();

    virtual void Render(const Display* const display) = 0;
    virtual glm::mat4 GetWorld();

private:
    glm::mat4 m_world;

};

#endif // _RENDEROBJECT_H
