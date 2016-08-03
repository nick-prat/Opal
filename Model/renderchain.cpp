#include <iostream>
#include <stdlib.h>
#include <malloc.h>

#include "Model/renderchain.h"

RenderChain* RenderChain::m_renderChain = nullptr;

RenderChain::RenderChain(int num, bool vol)
{
    m_memPool = nullptr;
    if(!InitRenderChain(num, vol))
    {
        std::cout << "Couldn't initialize render chain!" << std::endl;
        throw;
    }
}

RenderChain* RenderChain::GetInstance() {
    return m_renderChain;
}

bool RenderChain::CreateInstance(int num, bool vol) {
    if(m_renderChain != nullptr)
    {
        std::cout << "Render chain has already been created" << std::endl;
        return false;
    }
    m_renderChain = new RenderChain(num, vol);
    return true;
}

void RenderChain::DeleteInstance() {
    m_renderChain->Destroy();
    delete m_renderChain;
    m_renderChain = nullptr;
}

RenderChain::~RenderChain()
{
    delete m_renderChain;
    Destroy();
}

bool RenderChain::InitRenderChain(int num, bool vol)
{
    m_memPool = (RenderObject**)malloc(sizeof(RenderObject*) * num);
    m_objCount = 0;
    m_objLimit = num;
    m_volatile = vol;
    return true;
}

void RenderChain::Destroy()
{
    delete [] m_memPool;
    m_memPool = nullptr;
    m_objCount = 0;
    m_objLimit = 0;
}

bool RenderChain::AttachRenderObject(RenderObject* object)
{
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
    for(int i = 0; i < m_objCount; i++)
    {
        m_memPool[i]->Render();
    }
    if(m_volatile)
    {
        m_objCount = 0;
    }
}