#ifndef _RENDER_SYSTEM_H
#define _RENDER_SYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Render/shader.hpp>
#include <Render/light.hpp>
#include <ECS/system.hpp>

template<typename entity_manager_t>
class RenderSystem : public ISystem<RenderSystem<entity_manager_t>, entity_manager_t> {
    using isystem_t = ISystem<RenderSystem<entity_manager_t>, entity_manager_t>;
    using isystem_t::m_entityManager;
    using entity_t = Entity<entity_manager_t>;

public:
    RenderSystem(entity_manager_t* entityManager, const Shader& shader, const Display& display, const WorldLight& worldLight)
    : isystem_t(entityManager)
    , m_display(display)
    , m_shader(shader)
    , m_worldLight(worldLight) {
        static_assert(entity_manager_t::template contains<CRender>(), "RenderSystem requires invalid type CRender");
        static_assert(entity_manager_t::template contains<CLocation>(), "RenderSystem requires invalid type CLocation");
    }

    RenderSystem(const RenderSystem& renderSystem)
    : isystem_t(renderSystem.m_entityManager)
    , m_display(renderSystem.m_display)
    , m_shader(renderSystem.m_shader)
    , m_worldLight(renderSystem.m_worldLight) {}

    RenderSystem(RenderSystem&& renderSystem)
    : isystem_t(renderSystem.m_entityManager)
    , m_display(renderSystem.m_display)
    , m_shader(renderSystem.m_shader)
    , m_worldLight(renderSystem.m_worldLight) {}

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
    }*/

    // TODO Refactor this to actually use entities and components
    void update() {
        glUseProgram(m_shader.getProgram());

        GLint ambientLightLocation = m_shader.getUniformLocation("gAmbientLight");
        if(ambientLightLocation != -1) {
            glUniform4fv(ambientLightLocation, 1, glm::value_ptr(m_worldLight.getAmbientColor()));
        }

        for(const auto& object : m_shader.getRenderObjects()) {
            object->render(m_shader, m_display);
        }
    }

    void setDisplay(const Display* const display) {
        m_display = display;
    }

    std::size_t getRenderCount() const {
        return m_shader.getRenderCount();
    }

private:
    const Display& m_display;
    const Shader& m_shader;
    const WorldLight& m_worldLight;
};

#endif // _RENDER_SYSTEM_H
