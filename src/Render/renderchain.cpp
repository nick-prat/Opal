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
    if(object == nullptr) {
        Log::getErrorLog() << "Null object attached to render chain\n";
        return;
    }

    auto shader = object->getShader();
    if(shader == nullptr) {
        Log::getErrorLog() << "Render object has null shader\n";
        return;
    }

    if(m_objects.find(object->getShader()) == m_objects.end()) {
        m_objects[shader] = std::list<IRenderObject*>();
    }
    m_objects[shader].push_back(object);
}

void RenderChain::detach(IRenderObject* object) {
    m_objects[object->getShader()].remove(object);
}

void RenderChain::render(const Display* const display) const {
    for(const auto& shader : m_objects) {
        shader.first->useShader();
        for(const auto& object : shader.second) {
            object->render(display);
        }
    }
}

void RenderChain::clear() {
    m_objects.clear();
}

int RenderChain::getRenderCount() const {
    return m_objects.size();
}
