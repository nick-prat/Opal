#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include "renderobject.h"

class RenderChain
{
public:


    bool InitRenderChain(int numObjects, bool vol);
    bool InitRenderChain(int numObjects);
    void Destroy();

    bool AttachRenderObject(RenderObject* object);
    void RenderObjectChain();

    std::shared_ptr<RenderChain> getInstance();
    bool createInstance();
    bool createInstance(int num);
    bool createInstance(int num, bool vol);
    void deleteInstance();
protected:

private:
    RenderChain();
    RenderChain(int num);
    RenderChain(int num, bool vol);
    ~RenderChain();

private:
    static std::shared_ptr<RenderChain> m_renderChain;

    int m_objCount;
    int m_objLimit;
    bool m_volatile;
    RenderObject** m_memPool;
};

#endif // _RENDERCHAIN_H
