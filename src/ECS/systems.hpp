#ifndef _SYSTEMS_H
#define _SYSTEMS_H

#include <ECS/entity.hpp>

class IBaseSystem {
public:
    virtual void update() = 0;
};

template<typename system_t, typename entity_manager_t, typename... system_comp_ts>
class ISystem : public IBaseSystem {
public:
    ISystem(entity_manager_t* entityManager)
    : m_entityManager(entityManager) {}

    void update() {
        static_cast<system_t*>(this)->update();
    }

    void subscribe(Entity<entity_manager_t>* entity) {
        m_entities.push_back(entity);
    }

protected:
    entity_manager_t* m_entityManager;
    std::vector<Entity<entity_manager_t>*> m_entities;
};

#endif // _SYSTEMS_H
