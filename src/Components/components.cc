#include "components.hh"

#include <glm/gtx/transform.hpp>

CBody::CBody(const glm::vec3& loc)
: m_location(glm::translate(loc)) {}

void CBody::setLocation(const glm::vec3& loc) {
    m_location = glm::translate(loc);
}

void CBody::setLocation(const glm::mat4& loc) {
    m_location = loc;
}

const glm::mat4& CBody::getLocation() const {
    return m_location;
}

void CBody::setRotation(const glm::vec3& rotation) {
    // TODO Implement this
}

void CBody::setRotation(const glm::mat4& rotation) {
    m_rotate = rotation;
}

const glm::mat4& CBody::getRotation() const {
    return m_rotate;
}

void CBody::setScale(const glm::vec3& scale) {
    m_scale = glm::scale(scale);
}

void CBody::setScale(const glm::mat4& scale) {
    m_scale = scale;
}

const glm::mat4& CBody::getScale() const {
    return m_scale;
}

// CRender

CRender::CRender(const Model3D& model)
: m_model(model)
, m_vaos(m_model.generateVAOs()) {}

const std::vector<GLuint> CRender::getVAOs() const {
    return m_vaos;
}

const Model3D& CRender::getModel() const {
    return m_model;
}
