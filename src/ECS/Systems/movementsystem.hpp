#ifndef _MOVEMENT_SYSTEM_H
#define _MOVEMENT_SYSTEM_H

#include <ECS/components.hpp>
#include <ECS/system.hpp>

template<typename entity_manager_t>
class MovementSystem : public ISystem<MovementSystem<entity_manager_t>, entity_manager_t> {
public:
    static_assert(entity_manager_t::template contains<CLocation>(), "MovementSystem requires invalid type CLocation");
};

#endif // _MOVEMENT_SYSTEM_H
