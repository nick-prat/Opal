#include "camera.hpp"

#include <iostream>
#include <glm/gtx/transform.hpp>

#include <Utilities/exceptions.hpp>

Camera::Camera() {
    m_viewMatrix = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 5.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
}

Camera::~Camera() {
}

glm::mat4 Camera::GetViewMatrix() {
    return m_viewMatrix * glm::translate(m_translation);
}

void Camera::RotateCamera(glm::vec3 rotation) {
    m_rotation = m_rotation + rotation;
}

void Camera::MoveCamera(glm::vec3 delta) {
    m_translation = m_translation + delta;
}
