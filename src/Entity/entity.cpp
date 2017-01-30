#include "entity.hpp"

#include <glm/gtx/transform.hpp>
#include <Models/dynamicmodel.hpp>

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
        m_model->m_visible = visible;
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
        return m_model->m_visible;
    }
    return false;
}

std::string Entity::getName() const {
    return m_name;
}

DynamicModel* Entity::getModel() const {
    return m_model;
}

void Entity::translate(const glm::vec3& trans) {
    if(m_model != nullptr) {
        m_model->m_translate = glm::translate(m_model->m_translate, trans);
    } else {
        m_location = m_location + trans;
    }
}

void Entity::rotate(const float& degrees, const glm::vec3& rotate) {
    if(m_model != nullptr) {
        m_model->m_rotate = glm::rotate(m_model->m_rotate, degrees, rotate);
    }
}

void Entity::scale(const glm::vec3& scale) {
    if(m_model != nullptr) {
        m_model->m_scale = glm::scale(m_model->m_scale, scale);
    }
}
