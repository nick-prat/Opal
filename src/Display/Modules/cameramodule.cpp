#include <Display/display.hpp>

#include <iostream>

#include <Utilities/utilities.hpp>

using Utilities::Exception;

GlutDisplay::CameraModule::CameraModule() {
    m_viewMatrix = glm::mat4(1.0f);
    m_translation = glm::vec3(0.0f, 0.0f, 0.0f);
    m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    if(!InitCamera()) {
        throw Exception("Couldn't initialize camera!");
    }
}

GlutDisplay::CameraModule::~CameraModule() {
    Destroy();
}

bool GlutDisplay::CameraModule::InitCamera() {
    m_viewMatrix = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 5.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));

    return true;
}

void GlutDisplay::CameraModule::Destroy() {

}

glm::mat4 GlutDisplay::CameraModule::GetViewMatrix() {
    return m_viewMatrix * glm::translate(m_translation);
}

void GlutDisplay::CameraModule::RotateCamera(glm::vec3 rotation) {
    m_rotation = m_rotation + rotation;
}

void GlutDisplay::CameraModule::MoveCamera(glm::vec3 delta) {
    m_translation = m_translation + delta;
}
