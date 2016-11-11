#include <iostream>
#include <stdlib.h>
#include <malloc.h>

#include <Utilities/utilities.hpp>
#include <Utilities/log.hpp>
#include <Render/renderchain.hpp>

using Utilities::Exception;

RenderChain* RenderChain::m_renderChain = nullptr;

RenderChain*& RenderChain::GetInstance() {
    return m_renderChain;
}

bool RenderChain::CreateInstance(std::shared_ptr<GlutDisplay> display, bool vol) {
    if(m_renderChain != nullptr) {
        Log::error("Render chian has already been created", Log::OUT_CONS);
        return false;
    }

    m_renderChain = new RenderChain(display, vol);
    return true;
}

void RenderChain::DeleteInstance() {
    delete m_renderChain;
    m_renderChain = nullptr;
}

RenderChain::RenderChain(std::shared_ptr<GlutDisplay> display, bool vol)
        : m_display(display) {
    m_volatile = vol;
}

RenderChain::~RenderChain() {
    delete m_renderChain;
}

void RenderChain::AttachRenderObject(std::weak_ptr<IRenderObject> object) {
    if(object.lock() != nullptr) {
        m_objects.push_back(object);
    } else {
        throw Exception("Null param passed to attach render object");
    }
}

void RenderChain::RenderObjectChain() {
    for(std::weak_ptr<IRenderObject> object : m_objects) {
        auto obj = object.lock();

        if(obj == nullptr) {
            continue;
        }

        try {
            if(obj) {
                obj->Render(m_display);
            }
        } catch(Exception& error) {
            error.PrintError();
        }
    }
}
