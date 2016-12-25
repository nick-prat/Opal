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

void RenderChain::attach(IRenderObject* object) {
    m_objects.push_back(object);
}

void RenderChain::detach(int tag) {
    for(const auto& object : m_objects) {
        if(object->getTag() == tag) {
            m_objects.remove(object);
        }
    }
}

void RenderChain::render(const Display* const display) {
    for(const auto& object : m_objects) {
        object->render(display);
    }
}

void RenderChain::clear() {
    m_objects.clear();
}
