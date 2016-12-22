#include "entity.hpp"

Entity::Entity()
        : m_model(nullptr) {
}

Entity::Entity(DynamicModel* model)
        : m_model(model) {
}

Entity::~Entity() {

}

void Entity::SetVisible(const bool visible) {
    if(m_model != nullptr ) {
        m_model->SetVisible(visible);
    }
}

void Entity::SetName(const std::string& name) {
    m_name = name;
}

void Entity::UnbindModel() {
    m_model = nullptr;
}

void Entity::BindModel(DynamicModel* model) {
    m_model = model;
}

bool Entity::IsVisible() const {
    if(m_model != nullptr) {
        return m_model->IsVisible();
    }
    return false;
}

std::string Entity::GetName() const {
    return m_name;
}

DynamicModel* Entity::GetModel() const {
    return m_model;
}
