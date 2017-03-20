#ifndef _RENDER_SYSTEM_H
#define _RENDER_SYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Render/shader.hpp>
#include <ECS/systems.hpp>

template<typename entity_manager_t>
class RenderSystem : public ISystem<RenderSystem<entity_manager_t>, entity_manager_t> {
public:
    static_assert(entity_manager_t::template contains<CRender>(), "RenderSystem requires invalid type CRender");
    static_assert(entity_manager_t::template contains<CLocation>(), "RenderSystem requires invalid type CLocation");

    RenderSystem(entity_manager_t* entityManager, const Shader& shader, const Display& display)
    : ISystem<RenderSystem<entity_manager_t>, entity_manager_t>(entityManager)
    , m_display(display)
    , m_shader(shader)
    , m_ambientColor(glm::vec4(1.0f)) {}

    RenderSystem(const RenderSystem& renderSystem)
    : ISystem<RenderSystem<entity_manager_t>, entity_manager_t>(renderSystem.m_entityManager)
    , m_display(renderSystem.m_display)
    , m_shader(renderSystem.m_shader)
    , m_ambientColor(renderSystem.m_ambientColor) {}

    RenderSystem(RenderSystem&& renderSystem)
    : ISystem<RenderSystem<entity_manager_t>, entity_manager_t>(renderSystem.m_entityManager)
    , m_display(renderSystem.m_display)
    , m_shader(renderSystem.m_shader)
    , m_ambientColor(renderSystem.m_ambientColor) {}

    RenderSystem& operator=(const RenderSystem&) = delete;

    RenderSystem& operator=(RenderSystem&&) = delete;

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

    void start() {
        glUseProgram(m_shader.getProgram());

        GLint ambientLightLocation = m_shader.getUniformLocation("gAmbientLight");
        if(ambientLightLocation != -1) {
            glUniform4fv(ambientLightLocation, 1, glm::value_ptr(m_ambientColor));
        }

        for(const auto& object : m_shader.getRenderObjects()) {
            object->render(m_shader, m_display);
        }
    }

    void visit(Entity<entity_manager_t>& entity) {
        if(entity.template hasComponent<CRender>()) {
            std::cout << "yolo\n";
        }
    }

    void end() {

    }

    void setDisplay(const Display* const display) {
        m_display = display;
    }

    std::size_t getRenderCount() const {
        return m_shader.getRenderCount();
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
    const Display& m_display;
    const Shader& m_shader;
    glm::vec4 m_ambientColor;
};

#endif // _RENDER_SYSTEM_H
