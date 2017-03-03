#ifndef _ENTITY_H
#define _ENTITY_H

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <stack>
#include <iostream>
#include <functional>

#include <Utilities/exceptions.hpp>

class DynamicModel;

// NOTE What defines an entity vs a static object?
// TODO Implement error detection if a entity doesn't have a specified component
// TODO Implement component deletion on entity destructor
// TODO Implement static assert checks to clarify error messages

template <typename... comp_ts>
class EntityManager {
public:
    using entitymanager_t = EntityManager<comp_ts...>;
    static constexpr unsigned int parameter_size = sizeof...(comp_ts);

    template<typename comp_t>
    class Component {
        friend class EntityManager;
    public:
        inline comp_t* getComponent() {
            return &m_component;
        };

        inline bool isEnabled() {
            return m_enabled;
        }

        inline comp_t* operator->() {
            if(m_enabled) {
                return &m_component;
            } else {
                throw BadComponent(m_entityID, "operator-> called on disabled component");
            }
        }

        inline int getEntityID() {
            return m_entityID;
        }

    private:
        Component(int id)
        : m_entityID(id)
        , m_enabled(true) {};

    private:
        comp_t m_component;
        int m_entityID;
        bool m_enabled;
    };

    class Entity {
        friend class EntityManager;
    public:
        Entity() : m_id(-1) {};

        Entity(int id, EntityManager<comp_ts...>* entityManager)
        : m_entityManager(entityManager)
        , m_id(id) {};

        Entity(const Entity&) = delete;

        Entity(Entity&& ent)
        : m_entityManager(ent.m_entityManager)
        , m_id(ent.m_id) {
            ent.m_id = -1;
        }

        Entity& operator=(const Entity&) = delete;

        Entity& operator=(Entity&& ent) {
            m_entityManager = ent.m_entityManager;
            m_id = ent.m_id;
            ent.m_id = -1;
            return *this;
        }

        inline int getID() const {
            return m_id;
        }

        template<typename comp_t>
        void addComponent() {
            auto loc = index<comp_t>();
            if(loc != -1) {
                m_componentIDs[index<comp_t>()] = m_entityManager->createComponent<comp_t>(m_id);
            } else {
                throw BadComponent(m_id, "Attempted adding component to entity twice");
            }
        }

        template<typename comp_t>
        comp_t* getComponent() {
            return m_entityManager->getComponent<comp_t>(m_componentIDs[index<comp_t>()]);
        }

        template<typename comp_t>
        void removeComponent() {
            m_entityManager->removeComponent<comp_t>(m_id);
            m_componentIDs[index<comp_t>()] = -1;
        }

    private:
        EntityManager<comp_ts...>* m_entityManager;
        std::array<int, sizeof...(comp_ts)> m_componentIDs;
        int m_id;
    };

    int createEntity() {
        if(m_freeLocations.size() > 0) {
            auto loc = m_freeLocations.top();
            m_freeLocations.pop();
            m_entities[loc] = Entity(loc, this);
            return loc;
        } else {
            auto loc = m_entities.size();
            m_entities.push_back(Entity(loc, this));
            return loc;
        }
    }

    Entity* getEntity(int id) {
        if(id >= m_entities.size() || m_entities[id].getID() != id) {
            throw BadEntity(id, "Entity doesn't exist, can't return");
        } else {
            return &(m_entities[id]);
        }
    }

    void removeEntity(int id) {
        if(id >= m_entities.size() || m_entities[id].getID() == id) {
            m_entities[id] = Entity();
            m_freeLocations.push(id);
        } else {
            throw BadEntity(id, "Entity doesn't exist, can't remove");
        }
    }

    template<typename comp_t>
    std::vector<Component<comp_t>>& getComponentList() {
        return std::get<std::vector<Component<comp_t>>>(m_componentLists);
    }

    void registerService(std::function<void(void)>&& service) {
        m_services.push_back(service);
    }

    void updateServices() {
        for(auto& service : m_services) {
            service();
        }
    }

private:
    template<typename comp_t>
    int createComponent(int id) {
        auto& list = std::get<std::vector<Component<comp_t>>>(m_componentLists);
        list.push_back(Component<comp_t>(id));
        return list.size() - 1;
    }

    template<typename comp_t>
    comp_t* getComponent(int id) {
        auto& list = getComponentList<comp_t>();
        return &(list[id].m_component);
    }

    template<typename comp_t>
    void removeComponent(int id) {
        auto& compList = std::get<std::vector<Component<comp_t>>>(m_componentLists);
        compList[id].m_enabled = false;
        compList[id].m_entityID = -1;
        m_freeLocations.push(id);
    }

    template<typename T, typename U = void, typename... comp_ts1>
    static constexpr unsigned int index() {
        return std::is_same<T,U>::value ? 0 : 1 + index<T, comp_ts1...>();
    }

private:
    std::stack<unsigned int> m_freeLocations;
    std::vector<Entity> m_entities;
    std::vector<std::function<void(void)>> m_services;
    std::tuple<std::vector<Component<comp_ts>>...> m_componentLists;
};

#endif // _ENTITY_H
