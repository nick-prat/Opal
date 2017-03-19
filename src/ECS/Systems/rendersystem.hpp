#ifndef _RENDER_SYSTEM_H
#define _RENDER_SYSTEM_H

#include <glm/glm.hpp>

#include <Render/shader.hpp>
#include <ECS/systems.hpp>

template<typename entity_manager_t>
class RenderSystem : public ISystem<RenderSystem<entity_manager_t>, entity_manager_t> {
public:
    static_assert(entity_manager_t::template contains<CRender>(), "RenderSystem requires invalid type CRender");
    static_assert(entity_manager_t::template contains<CLocation>(), "RenderSystem requires invalid type CLocation");

    RenderSystem(entity_manager_t* entityManager)
    : ISystem<RenderSystem<entity_manager_t>, entity_manager_t>(entityManager)
    , m_ambientColor(glm::vec4(1.0f)) {}

    RenderSystem(const RenderSystem&) = delete;

    RenderSystem(RenderSystem&& renderSystem)
    : ISystem<RenderSystem<entity_manager_t>, entity_manager_t>(renderSystem.m_entityManager)
    , m_ambientColor(renderSystem.m_ambientColor)
    , m_shader(renderSystem.m_shader) {
        renderSystem.m_shader = nullptr;
    }

    RenderSystem& operator=(const RenderSystem&) = delete;

    RenderSystem& operator=(RenderSystem&& renderSystem) {
        m_ambientColor = renderSystem.m_ambientColor;
        m_shader = renderSystem.m_shader;

        renderSystem.m_ambientColor = glm::vec4(0.0f);
        renderSystem.m_shader = nullptr;

        return *this;
    }

    /* The old render function
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
    */

    void visit(Entity<entity_manager_t>& entity) {
        if(entity.template hasComponent<CRender>()) {
            std::cout << "yolo\n";
        }
    }

    void setDisplay(const Display* const display) {
        m_display = display;
    }

    void setShader(Shader* shader) {
        m_shader = shader;
    }

    std::size_t getRenderCount() const {
        return m_shader->getRenderCount();
    }

    void setAmbientColor(const glm::vec3& color) {
        m_ambientColor.r = color.r;
        m_ambientColor.g = color.g;
        m_ambientColor.b = color.b;
    }

    void setAmbientIntensity(const float intensity) {
        m_ambientColor.a = intensity;
    }

private:
    const Display* m_display;
    glm::vec4 m_ambientColor;
    Shader* m_shader;
};

#endif // _RENDER_SYSTEM_H
