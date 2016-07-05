#include <iostream>
#include <stdlib.h>
#include <malloc.h>

#include "Model/renderchain.h"
#include "Utilities/utilities.hpp"

using Utilities::SafeDelete;

RenderChain::RenderChain()
{
    m_memPool = nullptr;
}

RenderChain::RenderChain(int num)
{
    m_memPool = nullptr;
    if(!InitRenderChain(num))
    {
        std::cout << "Couldn't initialize render chain!" << std::endl;
        throw;
    }
}

RenderChain::RenderChain(int num, bool vol)
{
    m_memPool = nullptr;
    if(!InitRenderChain(num, vol))
    {
        std::cout << "Couldn't initialize render chain!" << std::endl;
        throw;
    }
}

RenderChain::~RenderChain()
{
    Destroy();
}

bool RenderChain::InitRenderChain(int num)
{
    return InitRenderChain(num, true);
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
    //SafeDelete(m_memPool);
    delete [] m_memPool;
    m_memPool = nullptr;
    m_objCount = 0;
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