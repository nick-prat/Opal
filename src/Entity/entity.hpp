#ifndef _ENTITY_H
#define _ENTITY_H

#include <vector>
#include <stack>
#include <functional>

#include <Utilities/utilities.hpp>

// NOTE What defines an entity vs a static object?
// TODO Implement error detection if a entity doesn't have a specified component
// TODO Implement component deletion on entity destructor
// TODO Implement static assert checks to clarify error messages

static constexpr unsigned int invalid_id = 0xFFFFFFFF;

template<typename entity_manager_t>
class Entity {
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

    template<typename comp_t>
    void addComponent() {
        auto loc = m_componentIDs[m_entityManager->template index<comp_t>()];
        if(loc == invalid_id) {
            m_componentIDs[m_entityManager->template index<comp_t>()] = m_entityManager->template createComponent<comp_t>(m_id);
        } else {
            throw BadComponent(m_id, "Attempted adding component to entity twice");
        }
    }

    template<typename comp_t>
    comp_t& getComponent() {
        return m_entityManager->template getComponent<comp_t>(m_entityManager->template index<comp_t>());
    }

    template<typename comp_t>
    void removeComponent() {
        m_entityManager->template removeComponent<comp_t>(m_id);
        m_componentIDs[m_entityManager->template index<comp_t>()] = invalid_id;
    }

    template<typename comp_t>
    bool hasComponent() {
        auto loc = m_componentIDs[m_entityManager->template index<comp_t>()];
        return (loc == invalid_id ? false : true);
    }

private:
    entity_manager_t* m_entityManager;
    std::array<unsigned int, entity_manager_t::parameter_size> m_componentIDs;
    unsigned int m_id;
};

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
        if(validate<system_comp_ts...>(entity)) {
            m_entities.push_back(entity);
        } else {
            throw BadEntity(entity->getID(), "Validation failed for subscribing entity to system");
        }
    }

private:
    template<typename comp_t = void, typename... comp_ts1>
    bool validate(Entity<entity_manager_t>* entity) {
        if(std::is_same<comp_t, void>::value) {
            return true;
        } else if(entity->template hasComponent<comp_t>()) {
            return validate<comp_ts1...>(entity);
        } else {
            return false;
        }
    }

protected:
    entity_manager_t* m_entityManager;
    std::vector<Entity<entity_manager_t>*> m_entities;
};

template<typename... comp_ts>
class EntityManager {
public:
    static constexpr unsigned int parameter_size = sizeof...(comp_ts);
    using entity_manager_t = EntityManager<comp_ts...>;

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
        return std::get<std::vector<Component<comp_t>>>(m_componentLists);
    }

    // TODO See if it's possible to use statis polymorphism only
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

private:
    std::stack<unsigned int> m_freeLocations;
    std::vector<Entity<entity_manager_t>> m_entities;
    std::vector<IBaseSystem*> m_systems;
    std::tuple<std::vector<Component<comp_ts>>...> m_componentLists;
};

#endif // _ENTITY_H
