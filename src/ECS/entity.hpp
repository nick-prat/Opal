#ifndef _ENTITY_H
#define _ENTITY_H

#include <vector>
#include <stack>
#include <functional>

#include <Utilities/utilities.hpp>

// NOTE What defines an entity vs a static object?
// TODO Implement component deletion on entity destructor

static constexpr unsigned int invalid_id = 0xFFFFFFFF;

template<typename entity_manager_t>
class Entity {
private:
    template<typename comp_t>
    static constexpr unsigned int index() {
        return entity_manager_t::template index<comp_t>();
    }

public:
    Entity()
    : m_entityManager(nullptr)
    , m_id(invalid_id) {
        for(auto& id : m_componentIDs) {
            id = invalid_id;
        }
    };

    Entity(int id, entity_manager_t* entityManager)
    : m_entityManager(entityManager)
    , m_id(id) {
        for(auto& id : m_componentIDs) {
            id = invalid_id;
        }
    };

    Entity(const Entity&) = delete;

    Entity(Entity&& ent)
    : m_entityManager(ent.m_entityManager)
    , m_id(ent.m_id) {
        ent.m_id = invalid_id;
        for(unsigned int i = 0; i < m_componentIDs.size(); i++) {
            m_componentIDs[i] = ent.m_componentIDs[i];
            ent.m_componentIDs[i] = invalid_id;
        }
    }

    Entity& operator=(const Entity&) = delete;

    Entity& operator=(Entity&& ent) {
        m_entityManager = ent.m_entityManager;
        ent.m_entityManager = nullptr;
        m_id = ent.m_id;
        ent.m_id = invalid_id;
        for(unsigned int i = 0; i < m_componentIDs.size(); i++) {
            m_componentIDs[i] = ent.m_componentIDs[i];
            ent.m_componentIDs[i] = invalid_id;
        }
        return *this;
    }

    unsigned int getID() const {
        return m_id;
    }

    // NOTE Good function to have in C++17
    /*template<typename comp_t = void, typename... comp_ts>
    void addComponents() {
        if constexpr(!std::is_same<comp_t, void>::value) {
            addComponent<comp_t>();
            addComponents<comp_ts...>();
        }
    }*/

    template<typename comp_t>
    void addComponent() {
        static_assert(entity_manager_t::template contains<comp_t>(), "addComponent called with invalid type");
        auto loc = m_componentIDs[index<comp_t>()];
        if(loc == invalid_id) {
            m_componentIDs[index<comp_t>()] = m_entityManager->template createComponent<comp_t>(m_id);
        } else {
            throw BadComponent(m_id, "Attempted adding component to entity twice");
        }
    }

    template<typename comp_t>
    comp_t& getComponent() {
        static_assert(entity_manager_t::template contains<comp_t>(), "getComponent called with invalid type");
        auto loc = m_componentIDs[index<comp_t>()];
        if(loc != invalid_id) {
            return m_entityManager->template getComponent<comp_t>(loc);
        } else {
            throw BadComponent(m_id, "Attempted getting component that doesn't exist from entity");
        }
    }

    template<typename comp_t>
    void removeComponent() {
        static_assert(entity_manager_t::template contains<comp_t>(), "removeComponent called with invalid type");
        auto loc = m_componentIDs[index<comp_t>()];
        if(loc != invalid_id) {
            m_entityManager->template removeComponent<comp_t>(m_componentIDs[index<comp_t>()]);
            m_componentIDs[index<comp_t>()] = invalid_id;
        } else {
            throw BadComponent(m_id, "Attempted removing component that doesn't exist from entity");
        }

    }

    template<typename comp_t>
    bool hasComponent() {
        static_assert(entity_manager_t::template contains<comp_t>(), "hasComponent called with invalid type");
        auto loc = m_componentIDs[index<comp_t>()];
        return (loc == invalid_id ? false : true);
    }

private:
    entity_manager_t* m_entityManager;
    std::array<unsigned int, entity_manager_t::parameter_size> m_componentIDs;
    unsigned int m_id;
};

#endif // _ENTITY_H
