#include <Opal/Systems/rendersystem.hh>
#include <Opal/Components/components.hh>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Opal::BoundingBoxRenderSystem::BoundingBoxRenderSystem(const Shader& shader, const Display& display, WorldLight& light)
: RenderSystem(shader, display, light) {}

void Opal::BoundingBoxRenderSystem::update(Emerald::EntityManager& entMan) {
    m_shader.useProgram();
    auto pv = m_display.getProjectionMatrix() * m_display.getCamera().getViewMatrix();
    entMan.mapEntities<CBody, CRender>([this, &entMan, &pv](auto ent) {

    });
}
