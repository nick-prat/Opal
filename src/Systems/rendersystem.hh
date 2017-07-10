#ifndef _RENDER_SYSTEM_H
#define _RENDER_SYSTEM_H

#include <Core/display.hh>

#include <Render/shader.hh>
#include <Render/light.hh>
#include <Emerald/emerald.hh>

template<typename system_t>
class RenderSystem : public Emerald::ISystem<system_t> {
public:
    RenderSystem(const Shader &shader, const Display &display, WorldLight &worldLight)
    : m_display(display)
    , m_shader(shader)
    , m_worldLight(worldLight) {}

    RenderSystem(const RenderSystem &renderSystem)
    : m_display(renderSystem.m_display)
    , m_shader(renderSystem.m_shader)
    , m_worldLight(renderSystem.m_worldLight) {}

    RenderSystem(RenderSystem &&renderSystem)
    : m_display(renderSystem.m_display)
    , m_shader(renderSystem.m_shader)
    , m_worldLight(renderSystem.m_worldLight) {}

    RenderSystem &operator=(const RenderSystem&) = delete;
    RenderSystem &operator=(RenderSystem&&) = delete;

    void setDisplay(const Display* const display) {
        m_display = display;
    }

    std::size_t getRenderCount() const {
        return this->m_entities.size();
    }

protected:
    const Display &m_display;
    const Shader &m_shader;
    WorldLight &m_worldLight;
};

class ModelRenderSystem : public RenderSystem<ModelRenderSystem> {
public:
    inline static const std::string shaderName = "shader_staticmodel";

    ModelRenderSystem(const Shader &shader, const Display &display, WorldLight &worldLight);
    void update(Emerald::EntityManager &entMan);

private:
    float m_angle;
};

class NormalRenderSystem : public RenderSystem<NormalRenderSystem> {
public:
    inline static const std::string shaderName = "shader_normal";

    NormalRenderSystem(const Shader &shader, const Display &display, WorldLight &worldLight);
    void update(Emerald::EntityManager &entMan);
};

class BoundingBoxRenderSystem : public RenderSystem<BoundingBoxRenderSystem> {
public:
    inline static const std::string shaderName = "shader_bbox";

    BoundingBoxRenderSystem(const Shader &shader, const Display &display, WorldLight &worldLight);
    void update(Emerald::EntityManager &entMan);
};

#endif // _RENDER_SYSTEM_H
