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

void RenderChain::Attach(std::weak_ptr<IRenderObject> object) {
    if(object.lock() != nullptr) {
        m_objects.push_back(object);
    } else {
        throw generic_exception("Null param passed to attach render object");
    }
}

void RenderChain::Render(const Display* const display) {
    for(const auto& object : m_objects) {
        try {
            auto obj = std::shared_ptr<IRenderObject>(object);
            obj->Render(display);
        } catch(std::bad_weak_ptr& error) {
            std::cout << error.what() << '\n';
        } catch(generic_exception& error) {
            error.PrintError();
        }
    }
}

void RenderChain::Clear() {
    m_objects.clear();
}
