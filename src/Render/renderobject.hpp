#ifndef _RENDEROBJECT_H
#define _RENDEROBJECT_H

#include <memory>

#include <Core/display.hpp>

class Shader;
class IRenderObject;

#include <Render/shader.hpp>

// NOTE Is there any other data I need to track for render objects?
// NOTE Are there any other functions every render object *should* have?

class IRenderObject
{
public:
    IRenderObject();
    IRenderObject(const IRenderObject&) = delete;
    IRenderObject(IRenderObject&&) = delete;
    virtual ~IRenderObject();

    IRenderObject& operator=(const IRenderObject&) = delete;
    IRenderObject& operator=(IRenderObject&&) = delete;

    static int getNumRenderObjects();

    virtual void render(const Shader& shader, const Display& display) const = 0;

private:
    static int m_numRenderObjects;
};

#endif // _RENDEROBJECT_H
