#include <iostream>
#include <stdlib.h>
#include <malloc.h>

#include "Model/renderchain.h"

RenderChain* RenderChain::m_renderChain = nullptr;

RenderChain* RenderChain::GetInstance() {
    return m_renderChain;
}

bool RenderChain::CreateInstance(int num, bool vol) {
    if(m_renderChain != nullptr)
    {
        std::cout << "Render chain has already been created" << std::endl;
        return false;
    }

    m_renderChain = new RenderChain(num, vol, std::this_thread::get_id());
    return true;
}

void RenderChain::DeleteInstance() {
    delete m_renderChain;
    m_renderChain = nullptr;
}

RenderChain::RenderChain(int num, bool vol, std::thread::id threadID)
{
    m_memPool = nullptr;
    m_memPool = (RenderObject**)malloc(sizeof(RenderObject*) * num);
    m_objCount = 0;
    m_objLimit = num;
    m_volatile = vol;
    m_threadID = threadID;
}

RenderChain::~RenderChain()
{
    delete m_renderChain;
    delete [] m_memPool;
    m_memPool = nullptr;
    m_objCount = 0;
    m_objLimit = 0;
}

bool RenderChain::AttachRenderObject(RenderObject* object)
{
    if(m_renderChain == nullptr)
    {
        return false;
    }

    // Make sure we don't go over the object render limit
    if(m_objCount < m_objLimit)
    {
        m_memPool[m_objCount] = object;
        m_objCount++;
        return true;
    }
    else
    {
        return false;
    }
}

void RenderChain::RenderObjectChain()
{
    // All rendering must be done on the creation thread
    std::thread::id tid = std::this_thread::get_id();
    if(m_renderChain != nullptr && m_renderChain->m_threadID == tid)
    {
        return;
    }

    for(int i = 0; i < m_objCount; i++)
    {
        m_memPool[i]->Render();
    }
    if(m_volatile)
    {
        m_objCount = 0;
    }
}
