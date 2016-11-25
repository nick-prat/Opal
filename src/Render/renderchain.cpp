#include <iostream>
#include <stdlib.h>
#include <malloc.h>

#include <Utilities/exceptions.hpp>
#include <Utilities/log.hpp>
#include <Render/renderchain.hpp>

RenderChain::RenderChain() {
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

void RenderChain::RenderObjectChain(const Display* const display) {
    for(const auto& object : m_objects) {
        std::shared_ptr<IRenderObject> obj;
        try {
            obj = std::shared_ptr<IRenderObject>(object);
        } catch(std::bad_weak_ptr& error) {
            continue;
        }

        try {
            if(obj) {
                obj->Render(display);
            }
        } catch(generic_exception& error) {
            error.PrintError();
        }
    }
}
