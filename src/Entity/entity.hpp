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
// TODO Implement component deletion, and implement it on entity destructor

template <typename... comp_ts>
class EntityManager {
    static constexpr int compSize = sizeof...(comp_ts);
public:
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

        /*template<typename comp_t>
        void removeComponent() {
            m_entityManager->removeComponent<comp_t>(m_id);
        }*/

    private:
        EntityManager<comp_ts...>* m_entityManager;
        std::tuple<comp_ts*...> m_components;
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

    Ent& getEntity(unsigned int id) {
        if(id >= m_entities.size() || m_entities[id].getID() != id) {
            throw BadEntity(id, "Entity doesn't exist, can't return");
        } else {
            return m_entities[id];
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
    std::vector<comp_t>& getComponentList() {
        return std::get<std::vector<comp_t>>(m_componentLists);
    }

    void registerService(std::function<void(EntityManager<comp_ts...>*)>&& service) {
        m_services.push_back((service));
    }

    void updateServices() {
        for(auto& service : m_services) {
            service(this);
        }
    }

private:
    template<typename comp_t>
    void createComponent(unsigned int id) {
        auto& map = std::get<std::unordered_map<unsigned int, comp_t*>>(m_componentLookup);
        if(map.find(id) == map.end()) {
            auto& compList = std::get<std::vector<comp_t>>(m_componentLists);
            compList.push_back(comp_t());
            map[id] = &(compList[compList.size()-1]);
        } else {
            throw BadComponent(id, "Component already exists for entity");
        }
    }

    template<typename comp_t>
    comp_t* getComponent(unsigned int id) {
        auto& map = std::get<std::unordered_map<unsigned int, comp_t*>>(m_componentLookup);
        if(map.find(id) != map.end()) {
            return map[id];
        } else {
            throw BadComponent(id, "Component doesn't eist for entity, can't return");
        }
    }

    /*template<typename comp_t>
    void removeComponent(unsigned int id) {
        auto& map = std::get<std::unordered_map<unsigned int, comp_t*>>(m_componentLookup);
        if(map.find(id) != map.end()) {
            auto& compList = std::get<std::vector<comp_t>>(m_componentLists);
            compList[id] = {};
            map.erase(id);
        } else {
            throw BadComponent(id, "Component doens't exist for entity, can't be removed");
        }
    }*/

private:
    std::stack<unsigned int> m_freeLocations;
    std::vector<Ent> m_entities;
    std::vector<std::function<void(EntityManager<comp_ts...>*)>> m_services;
    std::tuple<std::vector<comp_ts>...> m_componentLists;
    std::tuple<std::unordered_map<unsigned int, comp_ts*>...> m_componentLookup;
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
