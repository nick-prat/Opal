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

    void update() override {
        /*map([&](entity_t& ent) {
            auto& loc = ent.template getComponent<CLocation>();
            loc.setLocation(loc.getLocation() + loc.getDirection() * m_entityManager->getTimeScale());
        });*/
    }
};

#endif // _MOVEMENT_SYSTEM_H
