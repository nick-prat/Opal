#include "components.hpp"

// CLocationee

CLocation::CLocation(const glm::vec3& loc)
: m_location(loc) {
    std::cout << "reference const called\n";
}

void CLocation::setLocation(const glm::vec3& loc) {
    m_location = loc;
}

glm::vec3 CLocation::getLocation() const {
    return m_location;
}

void CLocation::setDirection(const glm::vec3& dir) {
    m_direction = dir;
}

glm::vec3 CLocation::getDirection() const {
    return m_direction;
}


// CRender

CRender::CRender(std::vector<GLuint>&& vaos)
: m_vaos(std::move(vaos)) {}

glm::mat4 CRender::getRotation() const {
    return m_rotate;
}

void CRender::setRotation(const glm::mat4& rotation) {
    m_rotate = rotation;
}

glm::mat4 CRender::getScale() const {
    return m_scale;
}

void CRender::setScale(const glm::mat4& scale) {
    m_scale = scale;
}
