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
class RenderSystem : public ISystem<RenderSystem<system_t>> {
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

    // TODO Refactor this to actually use entities and components
    void update() override {
        glUseProgram(m_shader.getProgram());

        GLint ambientLightLocation = m_shader.getUniformLocation("gAmbientLight");
        if(ambientLightLocation != -1) {
            glUniform4fv(ambientLightLocation, 1, glm::value_ptr(m_worldLight.getAmbientColor()));
        }

        for(const auto& object : m_shader.getRenderObjects()) {
            object->render(m_shader, m_display);
        }

        /*auto pv = m_display.getProjectionMatrix() * m_display.getCamera()->getViewMatrix();

        map([&](entity_t& ent) {
            auto& rc = ent.template getComponent<CRender>();
            auto& loc = ent.template getComponent<CLocation>();

            auto world = loc.getLocation() * rc.getRotation() * rc.getScale();
            auto mvp = pv * world;

            glUniform1i(m_shader.getUniformLocation("gSampler"), 0);
            glUniformMatrix4fv(m_shader.getUniformLocation("gMVP"), 1, GL_FALSE, glm::value_ptr(mvp));

            for(auto vao : rc.getVAOs()) {
                glBindVertexArray(vao);
            }

        });*/
    }

    void setDisplay(const Display* const display) {
        m_display = display;
    }

    std::size_t getRenderCount() const {
        return m_shader.getRenderCount();
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

    void update() {
        glUseProgram(m_shader.getProgram());

        GLint ambientLightLocation = m_shader.getUniformLocation("gAmbientLight");
        if(ambientLightLocation != -1) {
            glUniform4fv(ambientLightLocation, 1, glm::value_ptr(m_worldLight.getAmbientColor()));
        }

        for(const auto& object : m_shader.getRenderObjects()) {
            object->render(m_shader, m_display);
        }

        auto pv = m_display.getProjectionMatrix() * m_display.getCamera()->getViewMatrix();

        /*map([&](entity_t& ent) {
            auto& rc = ent.template getComponent<CRender>();
            auto& loc = ent.template getComponent<CLocation>();

            auto mvp = pv * loc.getLocation() * rc.getRotation() * rc.getScale();

            glUniform1i(m_shader.getUniformLocation("gSampler"), 0);
            glUniformMatrix4fv(m_shader.getUniformLocation("gMVP"), 1, GL_FALSE, glm::value_ptr(mvp));

            for(auto vao : rc.getVAOs) {
                glBindVertexArray(vao);
            }

        });*/
    }
};

#endif // _RENDER_SYSTEM_H
