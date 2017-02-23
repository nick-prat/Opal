#ifndef _ENTITY_H
#define _ENTITY_H

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <stack>
#include <iostream>

class DynamicModel;

// NOTE What defines an entity vs a static object?
// TOOD Implement error detection if a entity doesn't have a specified component

template <typename ... comp_ts>
class EntityManager {
    static constexpr int compSize = sizeof...(comp_ts);
public:
    class Ent {
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

        template<typename comp_t>
        void addComponent() {
            std::get<comp_t*>(m_components) = m_entityManager->createComponent<comp_t>();
        }

        template<typename comp_t>
        comp_t* getComponent() {
            return std::get<comp_t*>(m_components);
        }

    private:
        EntityManager<comp_ts...>* m_entityManager;
        std::tuple<comp_ts*...> m_components;
        unsigned int m_id;
    };

    EntityManager()
    : m_entities(10) {
        m_nextLocation.push(0);
    };

    unsigned int createEntity() {
        auto loc = m_nextLocation.top();
        if(loc >= m_entities.capacity()) {
            m_entities.resize(m_entities.size() * 2);
        }

        m_entities[loc] = Ent(loc, this);

        m_nextLocation.pop();
        if(m_nextLocation.size() == 0) {
            m_nextLocation.push(m_entities.size());
        }
        return loc;
    }

    Ent& getEntity(unsigned int id) {
        return m_entities[id];
    }

    template<typename comp_t>
    std::vector<comp_t> getComponentList() {
        return std::get<std::vector<comp_t>>(m_componentLists);
    }

private:
    template<typename comp_t>
    comp_t* createComponent() {
        auto& vec = std::get<std::vector<comp_t>>(m_componentLists);
        vec.push_back(comp_t());
        return &(vec[vec.size()-1]);
    }

private:
    std::stack<unsigned int> m_nextLocation;
    std::vector<Ent> m_entities;
    std::tuple<std::vector<comp_ts>...> m_componentLists;
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
