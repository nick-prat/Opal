#ifndef _ENTITY_H
#define _ENTITY_H

#include <Models/dynamicmodel.hpp>

class Entity {
public:
    Entity();
    ~Entity();

    void SetVisible(const bool visible);
    void SetRenderObject(const std::shared_ptr<DynamicModel> renderObject);
    void SetName(const std::string& name);

    bool IsVisible() const;
    std::string GetName() const;

private:
    std::string m_name;
    std::shared_ptr<DynamicModel> m_renderObject;
};

#endif // _ENTITY_H
