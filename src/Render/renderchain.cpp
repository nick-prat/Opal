#include <iostream>
#include <stdlib.h>
#include <malloc.h>

#include <Utilities/utilities.hpp>
#include <Utilities/log.hpp>
#include <Render/renderchain.hpp>

RenderChain* RenderChain::m_renderChain = nullptr;

RenderChain*& RenderChain::GetInstance() {
    return m_renderChain;
}

bool RenderChain::CreateInstance(bool vol) {
    if(m_renderChain != nullptr) {
        Log::error("Render chian has already been created", Log::OUT_CONS);
        return false;
    }

    m_renderChain = new RenderChain(vol);
    return true;
}

void RenderChain::DeleteInstance() {
    delete m_renderChain;
    m_renderChain = nullptr;
}

RenderChain::RenderChain(bool vol) {
    m_volatile = vol;
}

RenderChain::~RenderChain() {
    delete m_renderChain;
}

bool RenderChain::AttachRenderObject(std::weak_ptr<IRenderObject> object) {
    m_objects.push_back(object);
    return true;
}

void RenderChain::RenderObjectChain() {
    for(std::weak_ptr<IRenderObject> object : m_objects) {
        object.lock()->Render();
    }
}
