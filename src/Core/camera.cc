#include "camera.hh"

#include <iostream>
#include <glm/gtx/transform.hpp>

#include <Utilities/exceptions.hh>

// TODO Be able to bind camera position to an entity
// NOTE Should i be able to make the camera direction bind to an entity as well?

Camera::Camera() {
    m_position = glm::vec3(0.0f, 0.0f, 5.0f);
    m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
    m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_viewMatrix = glm::lookAt(m_position, m_position + m_direction, m_up);
}

Camera::~Camera() {
}

void Camera::update(float scale) {
    m_position += m_direction * scale;
}

// NOTE Do i need to build a look at matrix for every object that requests the view matrix?
glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_direction, m_up);
}

// TODO Implement camera rotation
// NOTE How far should the camera be able to move?
void Camera::rotateCamera(glm::vec3 rotation) {
    m_rotation += rotation;
    if(m_rotation.x > 0.9f) {
        m_rotation.x = 0.9f;
    } else if(m_rotation.x < -0.9f) {
        m_rotation.x = -0.9f;
    }
    m_direction = glm::vec3(
            glm::sin(m_rotation.y) * glm::cos(m_rotation.x),
            -glm::sin(m_rotation.x),
            -glm::cos(m_rotation.y) * glm::cos(m_rotation.x)
    );
}

void Camera::moveCamera(glm::vec3 delta) {
    m_position += delta;
}

void Camera::setPosition(glm::vec3 position) {
    m_position = position;
}

glm::vec3 Camera::getPosition() const {
    return m_position;
}

glm::vec3 Camera::getDirection() const {
    return m_direction;
}
