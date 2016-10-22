#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include "renderobject.hpp"

class RenderChain
{
public:
    bool AttachRenderObject(IRenderObject* object);
    void RenderObjectChain();

    static RenderChain*& GetInstance();
    static bool CreateInstance(int num, bool vol);
    static void DeleteInstance();

private:
    RenderChain(int num, bool vol);
    ~RenderChain();

private:
    static RenderChain* m_renderChain;

    int m_objCount;
    int m_objLimit;
    bool m_volatile;
    IRenderObject** m_memPool;
};

#endif // _RENDERCHAIN_H
