#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include "renderobject.h"

class RenderChain
{
public:
    bool InitRenderChain(int numObjects, bool vol);
    void Destroy();

    bool AttachRenderObject(RenderObject* object);
    void RenderObjectChain();

    static RenderChain* GetInstance();
    static bool CreateInstance(int num, bool vol);
    static void DeleteInstance();

protected:
    RenderChain(int num, bool vol);
    ~RenderChain();

private:


private:
    static RenderChain* m_renderChain;

    int m_objCount;
    int m_objLimit;
    bool m_volatile;
    RenderObject** m_memPool;
};

#endif // _RENDERCHAIN_H
