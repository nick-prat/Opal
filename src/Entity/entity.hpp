#ifndef _ENTITY_H
#define _ENTITY_H

#include <Render/renderobject.hpp>

class Entity {
public:
    Entity();
    ~Entity();

    void SetRenderObject(const std::shared_ptr<IRenderObject> renderObject);
    void SetName(const std::string& name);
    std::string GetName() const;

private:
    std::string m_name;
    std::shared_ptr<IRenderObject> m_renderObject;
};

#endif // _ENTITY_H
