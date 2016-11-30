#include "entity.hpp"

Entity::Entity() {

}

Entity::~Entity() {

}

void Entity::SetVisible(const bool visible) {
    m_renderObject->SetVisible(visible);
}

void Entity::SetName(const std::string& name) {
    m_name = name;
}

void Entity::SetRenderObject(const std::shared_ptr<DynamicModel> renderObject) {
    m_renderObject = renderObject;
}

bool Entity::IsVisible() const {
    return m_renderObject->IsVisible();
}

std::string Entity::GetName() const {
    return m_name;
}
