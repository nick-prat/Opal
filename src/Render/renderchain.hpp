#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include <memory>
#include <list>

#include <Render/renderobject.hpp>

class RenderChain {
public:
    bool AttachRenderObject(std::weak_ptr<IRenderObject> object);
    void RenderObjectChain();

    static RenderChain*& GetInstance();
    static bool CreateInstance(bool vol = false);
    static void DeleteInstance();

private:
    RenderChain(bool vol);
    ~RenderChain();

private:
    static RenderChain* m_renderChain;

    bool m_volatile;
    std::list<std::weak_ptr<IRenderObject>> m_objects;
};

#endif // _RENDERCHAIN_H
