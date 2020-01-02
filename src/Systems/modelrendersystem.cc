#include <Opal/Systems/rendersystem.hh>
#include <Opal/Components/components.hh>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Opal::ModelRenderSystem::ModelRenderSystem(const Shader& shader, const Display& display, WorldLight& worldLight)
: RenderSystem<ModelRenderSystem>{shader, display}
, m_worldLight{worldLight} {
    GLint sunUBOLoc = glGetUniformBlockIndex(m_shader.getProgram(), "SunLight");
    if(sunUBOLoc != -1) {
        glBindBufferBase(GL_UNIFORM_BUFFER, sunUBOLoc, m_worldLight.getSunUBO());
    }
}

void Opal::ModelRenderSystem::update(Emerald::EntityManager& entMan) {
    m_shader.useProgram();

    GLint ambientLightLocation = m_shader.getUniformLocation("gAmbientLight");
    if(ambientLightLocation != -1) {
        glUniform4fv(ambientLightLocation, 1, glm::value_ptr(m_worldLight.getAmbientColor()));
    }

    auto pv = m_display.getCamera().getProjectionMatrix() * m_display.getCamera().getViewMatrix();

    entMan.mapEntities<CRender, CBody>([this, &entMan, &pv] (auto ent) {
        auto [rc, loc] = entMan.getComponents<CRender, CBody>(ent);

        auto mvp = pv * (loc.getLocation() * loc.getRotation() * loc.getScale());

        glUniformMatrix4fv(m_shader.getUniformLocation("gMVP"), 1, GL_FALSE, glm::value_ptr(mvp));

        const auto& vaos = rc.getVAOs();
        const auto& model = rc.getModel();
        const auto samplerLoc = m_shader.getUniformLocation("gSampler");

        for(unsigned int i = 0; i < vaos.size(); i++) {
            model.getTexture(model.getMatName(i)).bind(0);
            glUniform1i(samplerLoc, model.getTexture(model.getMatName(i)).getTextureUnit());
            glBindVertexArray(vaos[i]);
            glDrawElements(GL_TRIANGLES, (GLsizei)model.getIndexCount(i), GL_UNSIGNED_INT, nullptr);
        }
    });

    entMan.mapEntities<CTerrain>([this, &entMan, &pv] (auto ent) {
        auto const& [terr] = entMan.getComponents<CTerrain>(ent);
        auto const& terrain = terr.terrain;

        auto const mvp = pv;

        glUniformMatrix4fv(m_shader.getUniformLocation("gMVP"), 1, GL_FALSE, glm::value_ptr(mvp));

        auto const samplerLoc = m_shader.getUniformLocation("gSampler");
        terrain.getTexture().bind(0);
        glUniform1i(samplerLoc, terrain.getTexture().getTextureUnit());

        glBindVertexArray(terrain.getVAO());
        glDrawElements(GL_TRIANGLES, (GLsizei)terrain.getIndexCount(), GL_UNSIGNED_INT, nullptr);
    });
}
