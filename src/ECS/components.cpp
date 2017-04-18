#include "components.hpp"

#include <glm/gtx/transform.hpp>

CLocation::CLocation(const glm::vec3& loc)
: m_location(glm::translate(loc)) {}

void CLocation::setLocation(const glm::vec3& loc) {
    m_location = glm::translate(loc);
}

void CLocation::setLocation(const glm::mat4& loc) {
    m_location = loc;
}

const glm::mat4& CLocation::getLocation() const {
    return m_location;
}

void CLocation::setDirection(const glm::vec3& dir) {
    m_direction = dir;
}

glm::vec3 CLocation::getDirection() const {
    return m_direction;
}


// CRender

CRender::CRender(const Model3D& model)
: m_model(model)
, m_vaos(m_model.generateVAOs()) {}

const glm::mat4& CRender::getRotation() const {
    return m_rotate;
}

void CRender::setRotation(const glm::mat4& rotation) {
    m_rotate = rotation;
}

const glm::mat4& CRender::getScale() const {
    return m_scale;
}

void CRender::setScale(const glm::mat4& scale) {
    m_scale = scale;
}

const std::vector<GLuint> CRender::getVAOs() const {
    return m_vaos;
}
