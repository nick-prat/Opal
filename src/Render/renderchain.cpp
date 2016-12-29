#include <Render/renderchain.hpp>

#include <Utilities/log.hpp>
#include <Render/Shader/shader.hpp>
#include <Render/renderobject.hpp>

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
    for(const auto& pair : m_objects) {
        pair.first->useShader();
        for(const auto& object : pair.second) {
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
