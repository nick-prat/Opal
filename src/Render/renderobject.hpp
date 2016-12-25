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

    static int GetNumRenderObjects();
    int GetTag();

private:
    static int m_numRenderObjects;
    int m_tag;
};

#endif // _RENDEROBJECT_H
