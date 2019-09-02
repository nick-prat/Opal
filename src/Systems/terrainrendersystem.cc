#include <Opal/Systems/rendersystem.hh>
#include <Opal/Components/components.hh>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Opal::TerrainRenderSystem::TerrainRenderSystem(Shader const& shader, Display const& display)
: RenderSystem<TerrainRenderSystem>{shader, display} {}

void Opal::TerrainRenderSystem::update(Emerald::EntityManager& entMan) {
    m_shader.useProgram();

    auto const pv = m_display.getCamera().getProjectionMatrix() * m_display.getCamera().getViewMatrix();

    entMan.mapEntities<CTerrain>([this, &entMan, &pv] (auto ent) {
        auto const& [terr] = entMan.getComponents<CTerrain>(ent);

        auto const mvp = pv;

        glUniformMatrix4fv(m_shader.getUniformLocation("gMVP"), 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(terr.vao);
        glDrawElements(GL_TRIANGLES, (GLsizei)terr.indexCount, GL_UNSIGNED_INT, nullptr);
    });
}
