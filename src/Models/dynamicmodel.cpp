#include "dynamicmodel.hpp"

DynamicModel::DynamicModel(const Model3D* const model3D)
        : StaticModel(model3D), m_visible(true) {
}

DynamicModel::~DynamicModel() {
}

void DynamicModel::render(const Display* const display) {
    if(m_visible) {
        StaticModel::render(display);
    }
}

glm::mat4 DynamicModel::generateMVP(const Display* const display) const {
    return display->GetProjectionMatrix() * display->GetCamera()->GetViewMatrix() * getWorld();
}

void DynamicModel::translate(const glm::vec3& trans)
{
    m_translate = glm::translate(m_translate, trans);
}

void DynamicModel::rotate(const float& degrees, const glm::vec3& rotate)
{
    m_rotate = glm::rotate(m_rotate, degrees, rotate);
}

void DynamicModel::scale(const glm::vec3& scale)
{
    m_scale = glm::scale(m_scale, scale);
}

void DynamicModel::setVisible(const bool visible) {
    m_visible = visible;
}

bool DynamicModel::isVisible() const {
    return m_visible;
}

glm::mat4 DynamicModel::getWorld() const {
    return m_translate * m_rotate * m_scale;
}
