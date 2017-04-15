#ifndef _MOVEMENT_SYSTEM_H
#define _MOVEMENT_SYSTEM_H

#include <ECS/components.hpp>
#include <ECS/system.hpp>

class MovementSystem : public ISystem<MovementSystem> {
public:
    MovementSystem() = default;
    MovementSystem(MovementSystem&&) = default;

    MovementSystem& operator=(const MovementSystem&) = delete;
    MovementSystem& operator=(MovementSystem&&) = delete;

    template<typename entity_manager_t>
    void update(entity_manager_t& entMan) {
        entMan.mapEntities(m_entities, [&entMan](auto& ent) {
            auto& loc = ent.template getComponent<CLocation>();
            loc.setLocation(loc.getLocation() + loc.getDirection() * entMan.getTimeScale());
        });
    }
};

#endif // _MOVEMENT_SYSTEM_H
