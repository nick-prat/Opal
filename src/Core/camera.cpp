#include "camera.hpp"

#include <iostream>
#include <glm/gtx/transform.hpp>

#include <Utilities/exceptions.hpp>

Camera::Camera() {
    m_position = glm::vec3(0.0f, 0.0f, 5.0f);
    m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::~Camera() {
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(m_position, m_position + m_direction, m_up);
}

void Camera::RotateCamera(glm::vec3 rotation) {
}

void Camera::MoveCamera(glm::vec3 delta) {
    m_position += delta;
}

void Camera::SetPosition(glm::vec3 position) {
    m_position = position;
}
