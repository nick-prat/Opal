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
    using entity_manager_t = EntityManager<comp_ts...>;
    static constexpr unsigned int invalid_id = 0xFFFFFFFF;
    static constexpr unsigned int parameter_size = sizeof...(comp_ts);

    class Entity {
        friend class EntityManager;
    public:
        Entity() : m_id(invalid_id) {};

        Entity(int id, EntityManager<comp_ts...>* entityManager)
        : m_entityManager(entityManager)
        , m_id(id) {};

        Entity(const Entity&) = delete;

        Entity(Entity&& ent)
        : m_entityManager(ent.m_entityManager)
        , m_id(ent.m_id) {
            ent.m_id = invalid_id;
        }

        Entity& operator=(const Entity&) = delete;

        Entity& operator=(Entity&& ent) {
            m_entityManager = ent.m_entityManager;
            m_id = ent.m_id;
            ent.m_id = invalid_id;
            return *this;
        }

        inline unsigned int getID() const {
            return m_id;
        }

        template<typename comp_t>
        void addComponent() {
            auto loc = m_componentIDs[index<comp_t, comp_ts...>()];
            if(loc != invalid_id) {
                m_componentIDs[index<comp_t, comp_ts...>()] = m_entityManager->createComponent<comp_t>(m_id);
            } else {
                throw BadComponent(m_id, "Attempted adding component to entity twice");
            }
        }

        template<typename comp_t>
        comp_t& getComponent() {
            return m_entityManager->getComponent<comp_t>(m_componentIDs[index<comp_t, comp_ts...>()]);
        }

        template<typename comp_t>
        void removeComponent() {
            m_entityManager->removeComponent<comp_t>(m_id);
            m_componentIDs[index<comp_t, comp_ts...>()] = invalid_id;
        }

        template<typename comp_t>
        bool hasComponent() {
            auto loc = m_componentIDs[index<comp_t, comp_ts...>()];
            return (loc == invalid_id ? false : true);
        }

    private:
        entity_manager_t* m_entityManager;
        std::array<unsigned int, parameter_size> m_componentIDs;
        unsigned int m_id;
    };

    template<typename comp_t>
    class Component {
        friend class EntityManager;
    public:
        inline comp_t& getComponent() {
            return m_component;
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

        inline unsigned int getEntityID() {
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

    template<typename system_t, typename... system_comp_ts>
    class System {
    public:
        System(entity_manager_t* entityManager)
        : m_entityManager(entityManager) {

        }

        void update() {
            static_cast<system_t*>(this)->update();
        }

        // TODO Validate entity has proper components
        void subscribe(entity_manager_t::Entity& entity) {
            if(validate<system_comp_ts...>(entity)) {

            } else {
                throw BadEntity(entity.getID(), "Validation failed for subscribing entity to system");
            }
        }

    private:
        template<typename comp_t = void, typename... comp_ts1>
        bool validate(entity_manager_t::Entity& entity) {
            if(std::is_same<comp_t, void>::value) {
                return true;
            } else if(entity.template hasComponent<comp_t>()) {
                return validate<comp_ts1...>(entity);
            } else {
                return false;
            }
        }

    private:
        entity_manager_t* m_entityManager;
    };

    unsigned int createEntity() {
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

    Entity& getEntity(unsigned int id) {
        if(id >= m_entities.size() || m_entities[id].getID() != id) {
            throw BadEntity(id, "Entity doesn't exist, can't return");
        } else {
            return m_entities[id];
        }
    }

    void removeEntity(unsigned int id) {
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

    void updateServices() {

    }

private:
    template<typename comp_t>
    int createComponent(int id) {
        auto& list = std::get<std::vector<Component<comp_t>>>(m_componentLists);
        list.push_back(Component<comp_t>(id));
        return list.size() - 1;
    }

    template<typename comp_t>
    comp_t& getComponent(unsigned int id) {
        auto& list = getComponentList<comp_t>();
        return list[id].m_component;
    }

    template<typename comp_t>
    void removeComponent(unsigned int id) {
        auto& compList = std::get<std::vector<Component<comp_t>>>(m_componentLists);
        compList[id].m_enabled = false;
        compList[id].m_entityID = invalid_id;
        m_freeLocations.push(id);
    }

    template<typename T, typename U = void, typename... comp_ts1>
    static constexpr unsigned int index(int i = 0) {
        if(std::is_same<U,void>::value) {
            throw GenericException("Index couldn't be found");
        } else if(std::is_same<T,U>::value) {
            return i;
        } else {
            return index<T,comp_ts1...>(++i);
        }
    }

private:
    std::stack<unsigned int> m_freeLocations;
    std::vector<Entity> m_entities;
    std::tuple<std::vector<Component<comp_ts>>...> m_componentLists;
};

#endif // _ENTITY_H
