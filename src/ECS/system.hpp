#ifndef _SYSTEMS_H
#define _SYSTEMS_H

#include <ECS/entity.hpp>
#include <ECS/components.hpp>

#include <iostream>
#include <unordered_set>

class IBaseSystem {
public:
    virtual ~IBaseSystem() {}
    virtual void update() = 0;
};

template<typename system_t, typename entity_manager_t>
class ISystem : public IBaseSystem {
    using entity_t = typename entity_manager_t::entity_t;
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

    void subscribe(entity_t* ent) {
        m_entities.inssert(ent);
    }

protected:
    entity_manager_t* m_entityManager;
    std::unordered_set<entity_t*> m_entities;
};

#endif // _SYSTEMS_H
