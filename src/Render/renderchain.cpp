#include <Render/renderchain.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <Utilities/log.hpp>
#include <Render/renderobject.hpp>

// NOTE How can i make this class thread safe

RenderChain::RenderChain()
        : m_ambientColor(glm::vec4(1.0f)) {
}

void RenderChain::attachShader(Shader* shader) {
    m_shaders.insert(shader);
}

void RenderChain::detachShader(Shader* shader) {
    m_shaders.erase(m_shaders.find(shader));
}

// NOTE Am i able to further optimize this process?
// NOTE Research more on open gl state changes, and their performance hits
void RenderChain::render(const Display* const display) const {
    for(const auto& shader : m_shaders) {
        glUseProgram(shader->m_shaderProgram);

        GLint ambientLightLocation = shader->getUniformLocation("gAmbientLight");
        if(ambientLightLocation != -1) {
            glUniform4fv(ambientLightLocation, 1, glm::value_ptr(m_ambientColor));
        }

        for(const auto& object : shader->m_renderObjects) {
            object->render(*shader, *display);
        }
    }
}

void RenderChain::clearShaders() {
    m_shaders.clear();
}

int RenderChain::getRenderCount() const {
    auto count = 0;
    for(const auto& shader : m_shaders) {
        count += shader->m_renderObjects.size();
    }
    return count;
}

void RenderChain::setAmbientColor(const glm::vec3& color) {
    m_ambientColor.r = color.r;
    m_ambientColor.g = color.g;
    m_ambientColor.b = color.b;
}

void RenderChain::setAmbientIntensity(const float intensity) {
    m_ambientColor.a = intensity;
}
