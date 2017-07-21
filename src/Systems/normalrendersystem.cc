#include "rendersystem.hh"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Components/components.hh>

NormalRenderSystem::NormalRenderSystem(const Shader &shader, const Display &display, WorldLight &worldLight)
: RenderSystem<NormalRenderSystem>(shader, display, worldLight) {}

void NormalRenderSystem::update(Emerald::EntityManager &entMan) {
    m_shader.useProgram();

    auto pv = m_display.getProjectionMatrix() * m_display.getCamera().getViewMatrix();
    entMan.mapEntities<CRender, CBody>([this, &entMan, &pv](auto ent) {
        auto &rc = entMan.getComponent<CRender>(ent);
        auto &loc = entMan.getComponent<CBody>(ent);

        auto mvp = pv * (loc.getLocation() * loc.getRotation() * loc.getScale());
        auto normalMatrix = glm::transpose(glm::inverse(glm::mat3x3(mvp)));

        glUniformMatrix3fv(m_shader.getUniformLocation("gNM"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
        glUniformMatrix4fv(m_shader.getUniformLocation("gMVP"), 1, GL_FALSE, glm::value_ptr(mvp));

        const auto &vaos = rc.getVAOs();
        const auto &model = rc.getModel();
        for(unsigned int i = 0; i < vaos.size(); i++) {
            const auto &mesh = model.getMesh(i);

            glBindVertexArray(vaos[i]);
            model.getTexture(mesh.getMatName()).bind();
            glDrawElements(GL_POINTS, (GLsizei)mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
        }
    });
}
