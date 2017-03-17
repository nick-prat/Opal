#ifndef _RENDER_SYSTEM_H
#define _RENDER_SYSTEM_H

#include <ECS/systems.hpp>
#include <ECS/components.hpp>

template<typename entity_manager_t>
class RenderSystem : public ISystem<RenderSystem<entity_manager_t>, entity_manager_t, CRender, CLocation> {
public:
    static_assert(entity_manager_t::template contains<CRender>(), "RenderSystem requires invalid type CRender");
    static_assert(entity_manager_t::template contains<CLocation>(), "RenderSystem requires invalid type CLocation");

    RenderSystem(entity_manager_t* entityManager)
    : ISystem<RenderSystem<entity_manager_t>, entity_manager_t, CRender, CLocation>(entityManager) {}

    void update() {

    }
};

#endif // _RENDER_SYSTEM_H
