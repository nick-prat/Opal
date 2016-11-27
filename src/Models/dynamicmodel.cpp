#include "dynamicmodel.hpp"

DynamicModel::DynamicModel(Model3D* model3D)
        : StaticModel(model3D) {
}

DynamicModel::~DynamicModel() {
}

const Model3D* const DynamicModel::GetModel() {
    return m_model.get();
}

glm::mat4 DynamicModel::GetWorld() const {
    return m_translate * m_rotate * m_scale;
}

glm::mat4 DynamicModel::GenerateMVP(const Display* const display) const {
    return display->GetProjectionMatrix() * display->GetCamera()->GetViewMatrix() * GetWorld();
}

void DynamicModel::Translate(const glm::vec3& trans)
{
    m_translate = glm::translate(m_translate, trans);
}

void DynamicModel::Rotate(const float& degrees, const glm::vec3& rotate)
{
    m_rotate = glm::rotate(m_rotate, degrees, rotate);
}

void DynamicModel::Scale(const glm::vec3& scale)
{
    m_scale = glm::scale(m_scale, scale);
}
