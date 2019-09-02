#ifndef _RENDER_SYSTEM_H
#define _RENDER_SYSTEM_H

#include <Opal/Core/display.hh>
#include <Opal/Render/shader.hh>
#include <Opal/Render/light.hh>

#include <Emerald/emerald.hh>

namespace Opal {

    template<typename system_t>
    class RenderSystem : public Emerald::ISystem<system_t> {
    public:
        RenderSystem(Shader const& shader, Display const& display)
        : m_display(display)
        , m_shader(shader) {}

        RenderSystem(RenderSystem const& renderSystem)
        : m_display(renderSystem.m_display)
        , m_shader(renderSystem.m_shader) {}

        RenderSystem(RenderSystem&& renderSystem)
        : m_display(renderSystem.m_display)
        , m_shader(renderSystem.m_shader) {}

        RenderSystem& operator=(RenderSystem const&) = delete;
        RenderSystem& operator=(RenderSystem&&) = delete;

        void setDisplay(Display const * const display) {
            m_display = display;
        }

        std::size_t getRenderCount() const {
            return this->m_entities.size();
        }

    protected:
        Display const& m_display;
        Shader const& m_shader;
    };

    class ModelRenderSystem : public RenderSystem<ModelRenderSystem> {
    public:
        inline static std::string const shaderName = "shader_staticmodel";

        ModelRenderSystem(Shader const& shader, Display const& display, WorldLight& worldLight);
        void update(Emerald::EntityManager& entMan) override final;

    private:
        WorldLight& m_worldLight;
    };

    class TerrainRenderSystem : public RenderSystem<TerrainRenderSystem> {
    public:
        inline static std::string const shaderName = "shader_terrain";

        TerrainRenderSystem(Shader const& shader, Display const& display);
        void update(Emerald::EntityManager& entMan) override final;
    };

    class NormalRenderSystem : public RenderSystem<NormalRenderSystem> {
    public:
        inline static std::string const shaderName = "shader_normal";

        NormalRenderSystem(Shader const& shader, Display const& display);
        void update(Emerald::EntityManager& entMan) override final;
    };

    class BoundingBoxRenderSystem : public RenderSystem<BoundingBoxRenderSystem> {
    public:
        inline static std::string const shaderName = "shader_bbox";

        BoundingBoxRenderSystem(Shader const& shader, Display const& display);
        void update(Emerald::EntityManager& entMan) override final;
    };

}

#endif // _RENDER_SYSTEM_H
