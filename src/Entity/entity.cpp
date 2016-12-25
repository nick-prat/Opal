#include "entity.hpp"

Entity::Entity()
        : m_model(nullptr) {
}

Entity::Entity(DynamicModel* model)
        : m_model(model) {
}

Entity::~Entity() {

}

void Entity::setVisible(const bool visible) {
    if(m_model != nullptr ) {
        m_model->setVisible(visible);
    }
}

void Entity::setName(const std::string& name) {
    m_name = name;
}

void Entity::unbindModel() {
    m_model = nullptr;
}

void Entity::bindModel(DynamicModel* model) {
    m_model = model;
}

bool Entity::isVisible() const {
    if(m_model != nullptr) {
        return m_model->isVisible();
    }
    return false;
}

std::string Entity::getName() const {
    return m_name;
}

DynamicModel* Entity::getModel() const {
    return m_model;
}
