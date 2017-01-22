#ifndef _RENDEROBJECT_H
#define _RENDEROBJECT_H

#include <memory>
#include <Render/renderchain.hpp>

class Shader;
class Display;

// NOTE Is there any other data I need to track for render objects?
// NOTE Are there any other functions every render object *should* have?

class IRenderObject
{
    friend class RenderChain;
public:
    IRenderObject();
    virtual ~IRenderObject();

    static int getNumRenderObjects();

protected:
    virtual void render(const Shader* const shader, const Display* const display) const = 0;

private:
    static int m_numRenderObjects;
};

#endif // _RENDEROBJECT_H
