#include <iostream>
#include <stdlib.h>
#include <malloc.h>

#include <Utilities/exceptions.hpp>
#include <Utilities/log.hpp>
#include <Render/renderchain.hpp>

RenderChain::RenderChain(std::shared_ptr<Display> display, bool vol)
        : m_display(display) {
    m_volatile = vol;
}

RenderChain::~RenderChain() {
}

void RenderChain::AttachRenderObject(std::weak_ptr<IRenderObject> object) {
    if(object.lock() != nullptr) {
        m_objects.push_back(object);
    } else {
        throw generic_exception("Null param passed to attach render object");
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
        } catch(generic_exception& error) {
            error.PrintError();
        }
    }
}
