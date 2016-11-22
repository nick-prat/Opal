#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include <memory>
#include <list>

#include <Core/display.hpp>
#include <Render/renderobject.hpp>

class RenderChain {
public:
    void AttachRenderObject(std::weak_ptr<IRenderObject> object);
    void RenderObjectChain();

    static RenderChain*& GetInstance();
    static bool CreateInstance(std::shared_ptr<Display> display, bool vol = false);
    static void DeleteInstance();

    void SetDisplay(std::shared_ptr<Display> display);

private:
    RenderChain(std::shared_ptr<Display> display, bool vol);
    ~RenderChain();

private:
    static RenderChain* m_renderChain;

    bool m_volatile;
    std::shared_ptr<Display> m_display;
    std::list<std::weak_ptr<IRenderObject>> m_objects;
};

#endif // _RENDERCHAIN_H
