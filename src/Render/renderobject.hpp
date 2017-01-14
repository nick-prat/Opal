#ifndef _RENDEROBJECT_H
#define _RENDEROBJECT_H

#include <memory>

class Shader;
class Display;

// NOTE Is there any other data I need to track for render objects?
// NOTE Are there any other functions every render object *should* have?

class IRenderObject
{
public:
    IRenderObject();
    virtual ~IRenderObject();

    virtual void render(const Display* const display) const = 0;

    static int getNumRenderObjects();
    void bindShader(Shader* shader);
    Shader* getShader();

protected:
    Shader* m_shader;

private:
    static int m_numRenderObjects;
};

#endif // _RENDEROBJECT_H
