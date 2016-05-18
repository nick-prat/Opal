#include "renderchain.h"

RenderChain::RenderChain()
{
    m_renderQueue = nullptr;
}

RenderChain::~RenderChain()
{
    m_renderQueue = nullptr;
}

void RenderChain::AttachRenderObject(RenderObject* object)
{
    RenderObjectContainer* container = new RenderObjectContainer();
    container->object = object;
    container->next = m_renderQueue;

    m_renderQueue = container;
}

void RenderChain::RenderObjectChain()
{
    RenderObjectContainer* item;
    while(m_renderQueue != nullptr)
    {
        item = m_renderQueue;
        item->object->Render();
        delete m_renderQueue;
        m_renderQueue = item->next;

    }
}

RenderChain::RenderObjectContainer::RenderObjectContainer()
{
    this->next = nullptr;
    this->object = nullptr;
}

RenderChain::RenderObjectContainer::~RenderObjectContainer()
{}

