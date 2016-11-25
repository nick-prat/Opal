#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include <memory>
#include <list>

#include <Core/display.hpp>
#include <Render/renderobject.hpp>

class RenderChain {
public:
    RenderChain();
    ~RenderChain();

    void AttachRenderObject(std::weak_ptr<IRenderObject> object);
    void RenderObjectChain(const Display* const display);

private:
    std::list<std::weak_ptr<IRenderObject>> m_objects;
};

#endif // _RENDERCHAIN_H
