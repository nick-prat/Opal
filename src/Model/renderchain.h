#ifndef _RENDERCHAIN_H
#define _RENDERCHAIN_H

#include <thread>

#include "renderobject.h"

class RenderChain
{
public:
    bool AttachRenderObject(RenderObject* object);
    void RenderObjectChain();

    static RenderChain* GetInstance();
    static bool CreateInstance(int num, bool vol);
    static void DeleteInstance();

private:
    RenderChain(int num, bool vol, std::thread::id);
    ~RenderChain();

private:
    static RenderChain* m_renderChain;

    std::thread::id m_threadID;
    int m_objCount;
    int m_objLimit;
    bool m_volatile;
    RenderObject** m_memPool;
};

#endif // _RENDERCHAIN_H
