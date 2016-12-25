#ifndef _ENTITY_H
#define _ENTITY_H

#include <Models/dynamicmodel.hpp>

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

private:
    std::string m_name;
    DynamicModel* m_model;
};

#endif // _ENTITY_H
