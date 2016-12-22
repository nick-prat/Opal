#ifndef _ENTITY_H
#define _ENTITY_H

#include <Models/dynamicmodel.hpp>

class Entity {
public:
    Entity();
    Entity(DynamicModel* model);
    ~Entity();

    void UnbindModel();
    void BindModel(DynamicModel* model);

    void SetVisible(const bool visible);
    void SetName(const std::string& name);

    bool IsVisible() const;
    std::string GetName() const;
    DynamicModel* GetModel() const;

private:
    std::string m_name;
    DynamicModel* m_model;
};

#endif // _ENTITY_H
