#ifndef _RENDEROBJECT_H
#define _RENDEROBJECT_H

#include <memory>

class Shader;
class Display;

class IRenderObject
{
public:
    IRenderObject();
    virtual ~IRenderObject();

    virtual void render(const Display* const display) const = 0;

    static int getNumRenderObjects();
    Shader* getShader();

protected:
    std::unique_ptr<Shader> m_shader;

private:
    static int m_numRenderObjects;
};

#endif // _RENDEROBJECT_H
