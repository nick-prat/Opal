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
        comp_t* getComponent() {
            return &m_component;
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

        unsigned int id() {
            return m_entityID;
        }

    private:
        Component(unsigned int id)
        : m_entityID(id)
        , m_enabled(true) {};

    private:
        comp_t m_component;
        unsigned int m_entityID;
        bool m_enabled;
    };

    class Ent {
        friend class EntityManager;
    public:
        Ent() : m_id(-1) {};

        Ent(unsigned int id, EntityManager<comp_ts...>* entityManager)
        : m_entityManager(entityManager)
        , m_id(id) {};

        Ent(const Ent&) = delete;

        Ent(Ent&& ent)
        : m_entityManager(ent.m_entityManager)
        , m_id(ent.m_id) {
            ent.m_id = -1;
        }

        Ent& operator=(const Ent&) = delete;

        Ent& operator=(Ent&& ent) {
            m_entityManager = ent.m_entityManager;
            m_id = ent.m_id;
            ent.m_id = -1;
            return *this;
        }

        inline unsigned int getID() const {
            return m_id;
        }

        template<typename comp_t>
        void addComponent() {
            m_entityManager->createComponent<comp_t>(m_id);
        }

        template<typename comp_t>
        comp_t* getComponent() {
            return m_entityManager->getComponent<comp_t>(m_id);
        }

        template<typename comp_t>
        void removeComponent() {
            m_entityManager->removeComponent<comp_t>(m_id);
        }

    private:
        EntityManager<comp_ts...>* m_entityManager;
        unsigned int m_id;
    };

    unsigned int createEntity() {
        if(m_freeLocations.size() > 0) {
            auto loc = m_freeLocations.top();
            m_freeLocations.pop();
            m_entities[loc] = Ent(loc, this);
            return loc;
        } else {
            auto loc = m_entities.size();
            m_entities.push_back(Ent(loc, this));
            return loc;
        }
    }

    Ent* getEntity(unsigned int id) {
        if(id >= m_entities.size() || m_entities[id].getID() != id) {
            throw BadEntity(id, "Entity doesn't exist, can't return");
        } else {
            return &(m_entities[id]);
        }
    }

    void removeEntity(unsigned int id) {
        if(id >= m_entities.size() || m_entities[id].getID() == id) {
            m_entities[id] = Ent();
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
        m_services.push_back((service));
    }

    void updateServices() {
        for(auto& service : m_services) {
            service();
        }
    }

private:
    template<typename comp_t>
    void createComponent(unsigned int id) {
        auto& map = m_componentMaps[index<comp_t, comp_ts...>()];
        if(map.find(id) == map.end()) {
            auto& compList = std::get<std::vector<Component<comp_t>>>(m_componentLists);
            compList.push_back(Component<comp_t>(id));
            map[id] = compList.size()-1;
        } else {
            throw BadComponent(id, "Component already exists for entity");
        }
    }

    template<typename comp_t>
    comp_t* getComponent(unsigned int id) {
        auto& map = m_componentMaps[index<comp_t, comp_ts...>()];
        if(map.find(id) != map.end()) {
            auto& list = getComponentList<comp_t>();
            return &(list[map[id]].m_component);
        } else {
            throw BadComponent(id, "Component doesn't eist for entity, can't return");
        }
    }

    template<typename comp_t>
    void removeComponent(unsigned int id) {
        auto& map = m_componentMaps[index<comp_t, comp_ts...>()];
        if(map.find(id) != map.end()) {
            auto& compList = std::get<std::vector<Component<comp_t>>>(m_componentLists);
            compList[map[id]].m_enabled = false;
            compList[map[id]].m_entityID = -1;
            map.erase(id);
            m_freeLocations.push(id);
        } else {
            throw BadComponent(id, "Component doesn't exist for entity, couldn't return");
        }
    }

    template<typename T, typename U = void, typename... comp_ts1>
    static constexpr unsigned int index() {
        return std::is_same<T,U>::value ? 0 : 1 + index<T, comp_ts1...>();
    }

private:
    static constexpr int compSize = sizeof...(comp_ts);

    std::stack<unsigned int> m_freeLocations;
    std::vector<Ent> m_entities;
    std::vector<std::function<void(void)>> m_services;
    std::tuple<std::vector<Component<comp_ts>>...> m_componentLists;
    std::array<std::unordered_map<unsigned int, unsigned int>, parameter_size> m_componentMaps;
};

class Entity {
public:
    Entity();
    Entity(DynamicModel* model);
    ~Entity();

    void unbindModel();
    void bindModel(DynamicModel* model);

    void setVisible(const bool visible);
    void setName(const std::string& name);

    bool isVisible() const;
    std::string getName() const;
    DynamicModel* getModel() const;

    void translate(const glm::vec3& trans);
    void rotate(const float degrees, const glm::vec3& rotate);
    void scale(const glm::vec3& scale);

private:
    glm::vec3 m_location;
    std::string m_name;
    DynamicModel* m_model;
};

#endif // _ENTITY_H
