#ifndef _ENTITY_MANAGER_H
#define _ENTITY_MANAGER_H

#include <vector>
#include <unordered_set>

#include <ECS/entity.hpp>
#include <ECS/system.hpp>
#include <Utilities/utilities.hpp>

template<typename... comp_ts>
class EntityManager {
public:
    static constexpr std::size_t parameter_size = sizeof...(comp_ts);
    using entity_manager_t = EntityManager<comp_ts...>;
    using entity_t = Entity<entity_manager_t>;

    template<typename comp_t>
    static constexpr bool contains() {
        return Utilities::contains<comp_t, comp_ts...>();
    }

    template<typename comp_t>
    static constexpr unsigned int index() {
        static_assert(contains<comp_t>(), "EntityManager::index() called on invalid type");
        return Utilities::index<comp_t, comp_ts...>();
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
        template<typename... args_t>
        Component(int id, args_t&&... args)
        : m_component(std::forward<args_t>(args)...)
        , m_entityID(id)
        , m_enabled(true) {};

    private:
        comp_t m_component;
        unsigned int m_entityID;
        bool m_enabled;
    };

    unsigned int createEntity() {
        if(m_freeLocations.size() > 0) {
            auto loc = m_freeLocations.top();
            m_freeLocations.pop();
            m_entities[loc] = entity_t(loc, this);
            return loc;
        } else {
            auto loc = m_entities.size();
            m_entities.push_back(entity_t(loc, this));
            return loc;
        }
    }

    entity_t& getEntity(unsigned int id) {
        if(id >= m_entities.size() || m_entities[id].getID() != id) {
            throw BadEntity(id, "Entity doesn't exist, can't return");
        } else {
            return m_entities[id];
        }
    }

    std::vector<entity_t>& getEntities() {
        return m_entities;
    }

    const std::vector<const entity_t>& getEntities() const {
        return m_entities;
    }

    const entity_t& getEntity(unsigned int id) const {
        if(id >= m_entities.size() || m_entities[id].getID() != id) {
            throw BadEntity(id, "Entity doesn't exist, can't return");
        } else {
            return m_entities[id];
        }
    }

    void removeEntity(unsigned int id) {
        if(id >= m_entities.size() || m_entities[id].getID() == id) {
            m_entities[id] = entity_t();
            m_freeLocations.push(id);
        } else {
            throw BadEntity(id, "Entity doesn't exist, can't remove");
        }
    }

    template<typename comp_t>
    const std::vector<Component<comp_t>>& getComponentList() const {
        static_assert(contains<comp_t>(), "EntityManager::getComponentList() called with invalid type");
        return std::get<std::vector<Component<comp_t>>>(m_componentLists);
    }

    template<typename comp_t, typename... args_t>
    int createComponent(int id, args_t&&... args) {
        static_assert(contains<comp_t>(), "EntityManager::createComponent() called with invalid type");
        auto& list = std::get<std::vector<Component<comp_t>>>(m_componentLists);
        list.push_back(Component<comp_t>(id, std::forward<args_t>(args)...));
        return list.size() - 1;
    }

    template<typename comp_t>
    comp_t& getComponent(unsigned int id) {
        static_assert(contains<comp_t>(), "EntityManager::getComponent() called with invalid type");
        auto& list = std::get<std::vector<Component<comp_t>>>(m_componentLists);
        return list[id].m_component;
    }

    template<typename comp_t>
    const comp_t& getComponent(unsigned int id) const {
        static_assert(contains<comp_t>(), "EntityManager::getComponent() const called with invalid type");
        auto& list = std::get<std::vector<Component<comp_t>>>(m_componentLists);
        return list[id].m_component;
    }

    template<typename comp_t>
    void removeComponent(unsigned int id) {
        static_assert(contains<comp_t>(), "EntityManager::removeComponent() called with invalid type");
        auto& compList = std::get<std::vector<Component<comp_t>>>(m_componentLists);
        compList[id].m_enabled = false;
        compList[id].m_entityID = invalid_id;
        m_freeLocations.push(id);
    }

    void attachSystem(IBaseSystem* system) {
        m_systems.insert(system);
    }

    void detachSystem(IBaseSystem* system) {
        m_systems.erase(m_systems.find(system));
    }

    void updateSystems(double timeScale) {
        m_timeScale = timeScale;
        for(auto& system : m_systems) {
            system->update();
        }
    }

    float getTimeScale() {
        return m_timeScale;
    }

private:
    float m_timeScale;
    std::stack<unsigned int> m_freeLocations;
    std::vector<entity_t> m_entities;
    std::unordered_set<IBaseSystem*> m_systems;
    std::tuple<std::vector<Component<comp_ts>>...> m_componentLists;
};

#endif // _ENTITY_MANGER_H
