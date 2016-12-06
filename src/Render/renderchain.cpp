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

void RenderChain::Attach(IRenderObject* object) {
    m_objects.push_back(object);
}

void RenderChain::Render(const Display* const display) {
    for(const auto& object : m_objects) {
        object->Render(display);
    }
}

void RenderChain::Clear() {
    m_objects.clear();
}
