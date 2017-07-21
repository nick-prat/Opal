#include "rendersystem.hh"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Components/components.hh>

BoundingBoxRenderSystem::BoundingBoxRenderSystem(const Shader &shader, const Display &display, WorldLight &light)
: RenderSystem(shader, display, light) {}

void BoundingBoxRenderSystem::update(Emerald::EntityManager &entMan) {
    m_shader.useProgram();
    auto pv = m_display.getProjectionMatrix() * m_display.getCamera().getViewMatrix();
    entMan.mapEntities<CBody, CRender>([this, &entMan, &pv](auto ent) {

    });
}
