#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include <list>
#include <unordered_map>

#include <Core/display.hpp>
#include <Render/renderobject.hpp>
#include <Render/Shader/shader.hpp>

class RenderChain {
public:
    RenderChain();
    ~RenderChain();

    void attach(IRenderObject* object);
    void detach(IRenderObject* object);
    void render(const Display* const display) const;
    void clear();
    int getRenderCount() const;

private:
    std::unordered_map<Shader*, std::list<IRenderObject*>> m_objects;
};

#endif // _RENDERCHAIN_H
