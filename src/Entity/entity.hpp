#ifndef _ENTITY_H
#define _ENTITY_H

#include <glm/glm.hpp>
#include <string>

class DynamicModel;

// NOTE What defines an entity vs a static object?

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
