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

    void Attach(std::weak_ptr<IRenderObject> object);
    void Render(const Display* const display);
    void Clear();

private:
    std::list<std::weak_ptr<IRenderObject>> m_objects;
};

#endif // _RENDERCHAIN_H
