#ifndef _RENDER_SYSTEM_H
#define _RENDER_SYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl3w.h>

#include <Core/display.hpp>
#include <Core/camera.hpp>
#include <Render/shader.hpp>
#include <Render/light.hpp>
#include <ECS/system.hpp>

template<typename system_t>
class RenderSystem : public ISystem<system_t> {
public:
    RenderSystem(const Shader& shader, const Display& display, const WorldLight& worldLight)
    : m_display(display)
    , m_shader(shader)
    , m_worldLight(worldLight) {}

    RenderSystem(const RenderSystem& renderSystem)
    : m_display(renderSystem.m_display)
    , m_shader(renderSystem.m_shader)
    , m_worldLight(renderSystem.m_worldLight) {}

    RenderSystem(RenderSystem&& renderSystem)
    : m_display(renderSystem.m_display)
    , m_shader(renderSystem.m_shader)
    , m_worldLight(renderSystem.m_worldLight) {}

    RenderSystem& operator=(const RenderSystem&) = delete;
    RenderSystem& operator=(RenderSystem&&) = delete;

    void setDisplay(const Display* const display) {
        m_display = display;
    }

    std::size_t getRenderCount() const {
        return this->m_entities.size();
    }

protected:
    const Display& m_display;
    const Shader& m_shader;
    const WorldLight& m_worldLight;
};

class ModelRenderSystem : public RenderSystem<ModelRenderSystem> {
public:
    ModelRenderSystem(const Shader& shader, const Display& display, const WorldLight& worldLight)
    : RenderSystem<ModelRenderSystem>(shader, display, worldLight) {}

    template<typename entity_manager_t>
    void update(entity_manager_t& entMan) {
        glUseProgram(m_shader.getProgram());

        GLint ambientLightLocation = m_shader.getUniformLocation("gAmbientLight");
        if(ambientLightLocation != -1) {
            glUniform4fv(ambientLightLocation, 1, glm::value_ptr(m_worldLight.getAmbientColor()));
        }

        auto pv = m_display.getProjectionMatrix() * m_display.getCamera()->getViewMatrix();

        entMan.mapEntities(m_entities, [this, &pv](Entity<entity_manager_t>& ent) {
            auto& rc = ent.template getComponent<CRender>();
            auto& loc = ent.template getComponent<CLocation>();

            auto mvp = pv * loc.getLocation() * rc.getRotation() * rc.getScale();

            glUniform1i(m_shader.getUniformLocation("gSampler"), 0);
            glUniformMatrix4fv(m_shader.getUniformLocation("gMVP"), 1, GL_FALSE, glm::value_ptr(mvp));

            for(auto vao : rc.getVAOs) {
                glBindVertexArray(vao);
            }
        });
    }
};

#endif // _RENDER_SYSTEM_H
