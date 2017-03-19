#ifndef _SYSTEMS_H
#define _SYSTEMS_H

#include <ECS/entity.hpp>
#include <ECS/components.hpp>

#include <iostream>

class IBaseSystem {
public:
    virtual void update() = 0;
    virtual void attach() = 0;
    virtual void detach() = 0;
};

template<typename system_t, typename entity_manager_t>
class ISystem : public IBaseSystem {
public:
    ISystem(entity_manager_t* entityManager)
    : m_active(false)
    , m_entityManager(entityManager) {}

    ~ISystem() {
        detach();
    }

    ISystem(const ISystem&) = delete;

    ISystem(ISystem<system_t, entity_manager_t>&& system)
    : m_active(false)
    , m_entityManager(system.m_entityManager)
    , m_entities(std::move(system.m_entities)){
        system.detach();
        system.m_entityManager = nullptr;
    }

    ISystem& operator=(const ISystem&) = delete;

    ISystem& operator=(ISystem&& system) {
        if(m_active) {
            detach();
        }

        m_entityManager = system.m_entityManager;
        m_entities = std::move(system.m_entities);
        system.detach();
        system.m_entityManager = nullptr;
    }

    void update() {
        auto& entityList = m_entityManager->getEntityList();
        for(auto& id : m_entities) {
            static_cast<system_t*>(this)->system_t::visit(entityList[id]);
        }
    }

    void subscribe(unsigned int id) {
        m_entities.push_back(id);
    }

    void attach() {
        std::cout << "attaching...\n";
        if(m_entityManager != nullptr && !m_active) {
            m_entityManager->attachSystem(this);
            m_active = true;
        }
    }

    void detach() {
        if(m_entityManager != nullptr && m_active) {
            m_entityManager->detachSystem(this);
            m_active = false;
        }
    }

protected:
    bool m_active;
    entity_manager_t* m_entityManager;
    std::vector<unsigned int> m_entities;
};

#endif // _SYSTEMS_H
