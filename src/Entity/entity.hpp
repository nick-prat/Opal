#ifndef _ENTITY_H
#define _ENTITY_H

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class DynamicModel;

// NOTE What defines an entity vs a static object?

template <typename ... comp_ts>
class Ent : public comp_ts... {
public:
    template <typename comp_t>
    inline comp_t& getAsComponent() {
        return static_cast<comp_t&>(*this);
    }
};

template <typename ... comp_ts>
class EntMan {
public:
    unsigned int createEntity() {
        auto tag = m_tagCounter++;
        m_entities[tag] = Ent<comp_ts...>();
        return tag;
    }

    Ent<comp_ts...>& getEntity(unsigned int tag) {
        return m_entities[tag];
    }

    void removeEntity(unsigned int tag) {
        if(m_entities.find(tag) != m_entities.end()) {
            m_entities.erase(tag);
        }
    }

private:
    unsigned int m_tagCounter = 0;
    std::unordered_map<unsigned int, Ent<comp_ts...>> m_entities;
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
