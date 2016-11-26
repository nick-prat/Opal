#include "entity.hpp"

Entity::Entity() {

}

Entity::~Entity() {

}

void Entity::SetName(const std::string& name) {
    m_name = name;
}

std::string Entity::GetName() const {
    return m_name;
}

void Entity::SetRenderObject(const std::shared_ptr<IRenderObject> renderObject) {
    m_renderObject = renderObject;
}
