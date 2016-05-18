#include <iostream>
#include <stdlib.h>

#include "renderchain.h"
#include "utilities.hpp"

RenderChain::RenderChain()
{
	m_memPool = nullptr;
}

RenderChain::~RenderChain()
{
    m_memPool = nullptr;
}

bool RenderChain::InitRenderChain(int num)
{
	m_memPool = (RenderObject**)malloc(sizeof(RenderObject*) * num);
	m_objCount = 0;
	m_objLimit = num;
	return true;
}

void RenderChain::Destroy()
{
	SafeDelete(m_memPool);
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
}