#include "rendersystem.hh"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Components/components.hh>

ModelRenderSystem::ModelRenderSystem(const Shader &shader, const Display &display, WorldLight &worldLight)
: RenderSystem<ModelRenderSystem>(shader, display, worldLight) {
    m_angle = 0.0f;
    GLint sunUBOLoc = glGetUniformBlockIndex(m_shader.getProgram(), "SunLight");
    if(sunUBOLoc != -1) {
        glBindBufferBase(GL_UNIFORM_BUFFER, sunUBOLoc, m_worldLight.getSunUBO());
    }
}

void ModelRenderSystem::update(Emerald::EntityManager &entMan) {
    m_angle += 0.05;

    m_worldLight.setSunLocation({glm::sin(m_angle) * 10.0f, glm::cos(m_angle) * 10.0f, 10.0f});

    glUseProgram(m_shader.getProgram());


    GLint ambientLightLocation = m_shader.getUniformLocation("gAmbientLight");
    if(ambientLightLocation != -1) {
        glUniform4fv(ambientLightLocation, 1, glm::value_ptr(m_worldLight.getAmbientColor()));
    }

    auto pv = m_display.getProjectionMatrix() * m_display.getCamera().getViewMatrix();

    entMan.mapEntities<CRender, CBody>([this, &entMan, &pv](auto ent) {
        auto &rc = entMan.getComponent<CRender>(ent);
        auto &loc = entMan.getComponent<CBody>(ent);

        auto mvp = pv * (loc.getLocation() * loc.getRotation() * loc.getScale());

        glUniform1i(m_shader.getUniformLocation("gSampler"), 0);
        glUniformMatrix4fv(m_shader.getUniformLocation("gMVP"), 1, GL_FALSE, glm::value_ptr(mvp));

        const auto &vaos = rc.getVAOs();
        const auto &model = rc.getModel();
        for(unsigned int i = 0; i < vaos.size(); i++) {
            const auto &mesh = model.getMesh(i);

            glBindVertexArray(vaos[i]);
            model.getTexture(mesh.getMatName()).bind();
            glDrawElements(GL_TRIANGLES, (GLsizei)mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
        }
    });
}
