#include "camera.hpp"

#include <iostream>
#include <glm/gtx/transform.hpp>

#include <Utilities/exceptions.hpp>

// TODO Be able to bind camera position to an entity
// NOTE Should i be able to make the camera direction bind to an entity as well?

Camera::Camera() {
    m_position = glm::vec3(0.0f, 0.0f, 5.0f);
    m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::~Camera() {
}

// NOTE Do i need to build a look at matrix for every object that requests the view matrix?
glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_direction, m_up);
}

// TODO Implement camera rotation
// NOTE How far should the camera be able to move?
void Camera::rotateCamera(glm::vec3 rotation) {
}

// TODO Implement function to move camera towards it's direction
void Camera::moveCamera(glm::vec3 delta) {
    m_position += delta;
}

// NOTE Why is this useful?
void Camera::setPosition(glm::vec3 position) {
    m_position = position;
}
