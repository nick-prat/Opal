#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include <memory>
#include <list>

#include <Core/display.hpp>
#include <Render/renderobject.hpp>

class RenderChain {
public:
    RenderChain(std::shared_ptr<Display> display, bool vol);
    ~RenderChain();

    void AttachRenderObject(std::weak_ptr<IRenderObject> object);
    void RenderObjectChain();

    void SetDisplay(std::shared_ptr<Display> display);

private:
    bool m_volatile;
    std::shared_ptr<Display> m_display;
    std::list<std::weak_ptr<IRenderObject>> m_objects;
};

#endif // _RENDERCHAIN_H
