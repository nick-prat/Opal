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

    MovementSystem &operator=(const MovementSystem&) = delete;
    MovementSystem &operator=(MovementSystem&&) = delete;

    void update(Emerald::EntityManager &entMan);
};

#endif // _MOVEMENT_SYSTEM_H
