#ifndef _MOVEMENT_SYSTEM_H
#define _MOVEMENT_SYSTEM_H

#include <ECS/components.hpp>
#include <ECS/system.hpp>

template<typename entity_manager_t>
class MovementSystem : public ISystem<MovementSystem<entity_manager_t>, entity_manager_t> {
    using isystem_t = ISystem<MovementSystem<entity_manager_t>, entity_manager_t>;
    using isystem_t::mapEntities;
    using isystem_t::m_entityManager;
    using entity_t = Entity<entity_manager_t>;

public:
    MovementSystem(entity_manager_t* entityManager)
    : isystem_t(entityManager) {
        static_assert(entity_manager_t::template contains<CLocation>(), "MovementSystem requires invalid type CLocation");
    }

    MovementSystem(MovementSystem&&) = default;

    MovementSystem& operator=(const MovementSystem&) = delete;
    MovementSystem& operator=(MovementSystem&&) = delete;

    void update() {
        mapEntities([&](entity_t& ent) {
            auto& loc = ent.template getComponent<CLocation>();
            loc.setLocation(loc.getLocation() + loc.getDirection() * m_entityManager->getTimeScale());
        });
    }
};

#endif // _MOVEMENT_SYSTEM_H
