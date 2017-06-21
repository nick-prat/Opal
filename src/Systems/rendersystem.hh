#ifndef _RENDER_SYSTEM_H
#define _RENDER_SYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Core/gl.hh>

#include <Core/camera.hh>
#include <Render/shader.hh>
#include <Render/light.hh>
#include <Emerald/emerald.hh>
#include <Utilities/log.hh>
#include <Components/components.hh>

template<typename system_t>
class RenderSystem : public Emerald::ISystem<system_t> {
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
    inline static const std::string shaderName = "shader_staticmodel";

    ModelRenderSystem(const Shader& shader, const Display& display, const WorldLight& worldLight)
    : RenderSystem<ModelRenderSystem>(shader, display, worldLight) {}

    void update(Emerald::EntityManager& entMan) {
        glUseProgram(m_shader.getProgram());

        GLint ambientLightLocation = m_shader.getUniformLocation("gAmbientLight");
        if(ambientLightLocation != -1) {
            glUniform4fv(ambientLightLocation, 1, glm::value_ptr(m_worldLight.getAmbientColor()));
        }

        auto pv = m_display.getProjectionMatrix() * m_display.getCamera()->getViewMatrix();

        auto rview = entMan.getComponentView<CRender>();
        auto lview = entMan.getComponentView<CBody>();
        entMan.mapEntities<CRender, CBody>([this, &rview, &lview, &entMan, &pv](auto ent) {
            auto& rc = entMan.getComponent<CRender>(ent);
            auto& loc = entMan.getComponent<CBody>(ent);

            auto mvp = pv * (loc.getLocation() * loc.getRotation() * loc.getScale());

            glUniform1i(m_shader.getUniformLocation("gSampler"), 0);
            glUniformMatrix4fv(m_shader.getUniformLocation("gMVP"), 1, GL_FALSE, glm::value_ptr(mvp));

            const auto& vaos = rc.getVAOs();
            const auto& model = rc.getModel();
            for(unsigned int i = 0; i < vaos.size(); i++) {
                const auto& mesh = model.getMesh(i);

                glBindVertexArray(vaos[i]);
                model.getTexture(mesh.getMatName()).bind();
                glDrawElements(GL_TRIANGLES, (GLsizei)mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
            }
        });
    }
};

#endif // _RENDER_SYSTEM_H
