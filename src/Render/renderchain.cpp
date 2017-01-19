#include <Render/renderchain.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <Utilities/log.hpp>
#include <Resources/shader.hpp>
#include <Render/renderobject.hpp>

// NOTE How can i make this class thread safe

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

// NOTE Am i able to further optimize this process?
// NOTE Research more on open gl state changes, and their performance hits
void RenderChain::render(const Display* const display) const {
    for(const auto& pair : m_objects) {
        pair.first->useShader();

        GLint ambientLightLocation = pair.first->getUniformLocation("gAmbientLight");
        if(ambientLightLocation != -1) {
            glUniform4f(ambientLightLocation, m_ambientColor.r, m_ambientColor.g, m_ambientColor.b, m_ambientIntensity);
        }

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

void RenderChain::setAmbientColor(const glm::vec3& color) {
    m_ambientColor = color;
}

void RenderChain::setAmbientIntensity(const float intensity) {
    m_ambientIntensity = intensity;
}
