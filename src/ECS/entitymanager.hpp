#ifndef _ENTITY_MANAGER_H
#define _ENTITY_MANAGER_H

#include <vector>

#include <ECS/entity.hpp>
#include <ECS/systems.hpp>
#include <Utilities/utilities.hpp>

template<typename... comp_ts>
class EntityManager {
public:
    static constexpr unsigned int parameter_size = sizeof...(comp_ts);
    using entity_manager_t = EntityManager<comp_ts...>;

    template<typename comp_t>
    static constexpr bool contains() {
        return Utilities::contains<comp_t, comp_ts...>();
    }

    template<typename comp_t>
    class Component {
        friend class EntityManager;
    public:
        comp_t& getComponent() {
            return m_component;
        };

        bool isEnabled() {
            return m_enabled;
        }

        comp_t* operator->() {
            if(m_enabled) {
                return &m_component;
            } else {
                throw BadComponent(m_entityID, "operator-> called on disabled component");
            }
        }

        unsigned int getEntityID() {
            return m_entityID;
        }

    private:
        Component(int id)
        : m_entityID(id)
        , m_enabled(true) {};

    private:
        comp_t m_component;
        unsigned int m_entityID;
        bool m_enabled;
    };

    template<typename comp_t>
    unsigned int index() {
        static_assert(contains<comp_t>(), "index() called on invalid type");
        return Utilities::index<comp_t, comp_ts...>();
    }

    unsigned int createEntity() {
        if(m_freeLocations.size() > 0) {
            auto loc = m_freeLocations.top();
            m_freeLocations.pop();
            m_entities[loc] = Entity<entity_manager_t>(loc, this);
            return loc;
        } else {
            auto loc = m_entities.size();
            m_entities.push_back(Entity<entity_manager_t>(loc, this));
            return loc;
        }
    }

    Entity<entity_manager_t>& getEntity(unsigned int id) {
        if(id >= m_entities.size() || m_entities[id].getID() != id) {
            throw BadEntity(id, "Entity doesn't exist, can't return");
        } else {
            return m_entities[id];
        }
    }

    void removeEntity(unsigned int id) {
        if(id >= m_entities.size() || m_entities[id].getID() == id) {
            m_entities[id] = Entity<entity_manager_t>();
            m_freeLocations.push(id);
        } else {
            throw BadEntity(id, "Entity doesn't exist, can't remove");
        }
    }

    template<typename comp_t>
    std::vector<Component<comp_t>>& getComponentList() {
        static_assert(contains<comp_t>(), "getComponentList called with invalid type");
        return std::get<std::vector<Component<comp_t>>>(m_componentLists);
    }

    // TODO See if it's possible to use static polymorphism only
    void registerSystem(IBaseSystem* system) {
        m_systems.push_back(system);
    }

    void updateSystems() {
        for(auto& system : m_systems) {
            system->update();
        }
    }

    template<typename comp_t>
    int createComponent(int id) {
        static_assert(contains<comp_t>(), "createComponent called with invalid type");
        auto& list = std::get<std::vector<Component<comp_t>>>(m_componentLists);
        list.push_back(Component<comp_t>(id));
        return list.size() - 1;
    }

    template<typename comp_t>
    comp_t& getComponent(unsigned int id) {
        static_assert(contains<comp_t>(), "getComponent called with invalid type");
        auto& list = getComponentList<comp_t>();
        return list[id].m_component;
    }

    template<typename comp_t>
    void removeComponent(unsigned int id) {
        static_assert(contains<comp_t>(), "removeComponent called with invalid type");
        auto& compList = std::get<std::vector<Component<comp_t>>>(m_componentLists);
        compList[id].m_enabled = false;
        compList[id].m_entityID = invalid_id;
        m_freeLocations.push(id);
    }

private:
    std::stack<unsigned int> m_freeLocations;
    std::vector<Entity<entity_manager_t>> m_entities;
    std::vector<IBaseSystem*> m_systems;
    std::tuple<std::vector<Component<comp_ts>>...> m_componentLists;
};

#endif // _ENTITY_MANGER_H
