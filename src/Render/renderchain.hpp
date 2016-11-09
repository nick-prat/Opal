#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include <memory>
#include <list>

#include <Display/display.hpp>
#include <Render/renderobject.hpp>

class RenderChain {
public:
    bool AttachRenderObject(std::weak_ptr<IRenderObject> object);
    void RenderObjectChain();

    static RenderChain*& GetInstance();
    static bool CreateInstance(std::shared_ptr<GlutDisplay> display, bool vol = false);
    static void DeleteInstance();

    void SetDisplay(std::shared_ptr<GlutDisplay> display);

private:
    RenderChain(std::shared_ptr<GlutDisplay> display, bool vol);
    ~RenderChain();

private:
    static RenderChain* m_renderChain;

    bool m_volatile;
    std::shared_ptr<GlutDisplay> m_display;
    std::list<std::weak_ptr<IRenderObject>> m_objects;
};

#endif // _RENDERCHAIN_H
