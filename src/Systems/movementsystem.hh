#ifndef _MOVEMENT_SYSTEM_H
#define _MOVEMENT_SYSTEM_H

#include <glm/gtx/transform.hpp>

#include <Emerald/emerald.hh>
#include <Components/components.hh>
#include <Emerald/system.hh>

class MovementSystem : public Emerald::ISystem<MovementSystem> {
public:
    MovementSystem() = default;
    MovementSystem(MovementSystem&&) = default;

    MovementSystem& operator=(const MovementSystem&) = delete;
    MovementSystem& operator=(MovementSystem&&) = delete;

    void update(Emerald::EntityManager& entMan) {
        auto lview = entMan.getComponentView<CBody>();
        entMan.mapEntities<CBody>([&entMan, &lview](auto ent) {
            auto& loc = entMan.getComponent<CBody>(ent);
            // loc.setLocation(glm::translate(loc.getLocation(), loc.getDirection()));
        });
    }
};

#endif // _MOVEMENT_SYSTEM_H
