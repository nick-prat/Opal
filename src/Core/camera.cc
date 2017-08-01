#include "camera.hh"

#include <iostream>
#include <glm/gtx/transform.hpp>

#include <Util/exceptions.hh>

// TODO Be able to bind camera position to an entity
// NOTE Should i be able to make the camera direction bind to an entity as well?

Opal::Camera::Camera() {
    m_position = glm::vec3(0.0f, 0.0f, 5.0f);
    m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
    m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_viewMatrix = glm::lookAt(m_position, m_position + m_direction, m_up);
}

Opal::Camera::~Camera() {

}

void Opal::Camera::update(float scale) {
    m_position += m_direction * scale;
}

// NOTE Do i need to build a look at matrix for every object that requests the view matrix?
glm::mat4 Opal::Camera::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_direction, m_up);
}

// TODO Implement camera rotation
// NOTE How far should the camera be able to move?
void Opal::Camera::rotateCamera(glm::vec3 rotation) {
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

void Opal::Camera::moveCamera(glm::vec3 delta) {
    m_position += delta;
}

void Opal::Camera::setPosition(glm::vec3 position) {
    m_position = position;
}

glm::vec3 Opal::Camera::getPosition() const {
    return m_position;
}

glm::vec3 Opal::Camera::getDirection() const {
    return m_direction;
}
