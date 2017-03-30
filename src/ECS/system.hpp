#ifndef _SYSTEMS_H
#define _SYSTEMS_H

#include <ECS/entity.hpp>
#include <ECS/components.hpp>

#include <iostream>

class IBaseSystem {
public:
    virtual ~IBaseSystem() {}
    virtual void update() = 0;
};

template<typename system_t, typename entity_manager_t>
class ISystem : public IBaseSystem {
public:
    ISystem(entity_manager_t* entityManager)
    : m_entityManager(entityManager) {}

    virtual ~ISystem() {}

    ISystem(const ISystem&) = delete;

    ISystem(ISystem<system_t, entity_manager_t>&& system)
    : m_entityManager(system.m_entityManager) {
        system.m_entityManager = nullptr;
    }

    ISystem& operator=(const ISystem&) = delete;
    ISystem& operator=(ISystem&& system) = delete;

    void update() override {
        static_cast<system_t*>(this)->update();
    }

protected:
    entity_manager_t* m_entityManager;
};

#endif // _SYSTEMS_H
