#include <Opal/Core/camera.hh>
#include <Opal/Util/exceptions.hh>

#include <iostream>
#include <glm/gtx/transform.hpp>

// TODO Be able to bind camera position to an entity
// NOTE Should i be able to make the camera direction bind to an entity as well?

Opal::Camera::Camera()
: m_position{0.0f, 0.0f, 5.0f}
, m_direction{0.0f, 0.0f, -1.0f}
, m_rotation{0.0f, 0.0f, 0.0f}
, m_up{0.0f, 1.0f, 0.0f}
, m_rotationClamp{-0.9f, 0.9f}
, m_viewMatrix{glm::lookAt(m_position, m_position + m_direction, m_up)} {}

void Opal::Camera::update(const float scale) {
    m_position += m_direction * scale;
}

// NOTE Do i need to build a look at matrix for every object that requests the view matrix?
glm::mat4 Opal::Camera::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_direction, m_up);
}

// TODO Implement camera rotation
// NOTE How far should the camera be able to move?
void Opal::Camera::rotateCamera(const glm::vec3 rotation) {
    m_rotation += rotation;
    m_rotation.x = glm::clamp(m_rotation.x, m_rotationClamp.x, m_rotationClamp.y);
    m_direction = glm::vec3(
            glm::sin(m_rotation.y) * glm::cos(m_rotation.x),
            -glm::sin(m_rotation.x),
            -glm::cos(m_rotation.y) * glm::cos(m_rotation.x)
    );
}

void Opal::Camera::moveCamera(const glm::vec3 delta) {
    m_position += delta;
}

void Opal::Camera::setPosition(const glm::vec3 position) {
    m_position = position;
}

void Opal::Camera::setRotationClamp(const glm::vec2 clamp) {
    m_rotationClamp = clamp;
}

glm::vec3 Opal::Camera::getPosition() const {
    return m_position;
}

glm::vec3 Opal::Camera::getDirection2D() const {
    return {m_direction.x, 0.0f, m_direction.z};
}

glm::vec3 Opal::Camera::getDirection() const {
    return m_direction;
}

glm::vec3 Opal::Camera::getUpDirection() const {
    return m_up;
}
