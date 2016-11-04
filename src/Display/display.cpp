#include "Display/display.hpp"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Core/glapi.hpp>
#include <Utilities/utilities.hpp>

using Utilities::Exception;

GlutDisplay::GlutDisplay() {
    m_inputModule = nullptr;
    m_projMatrix = glm::mat4(1.0f);
}

GlutDisplay::GlutDisplay(int width, int height)
    : m_projMatrix(glm::mat4(1.0f)) {
    if(!InitDisplay(width, height))
    {
        throw Exception("Couldn't init display");
    }
}

GlutDisplay::~GlutDisplay() {
    Destroy();
}

glm::mat4 GlutDisplay::GetProjectionMatrix() {
    return m_projMatrix;
}

bool GlutDisplay::InitDisplay(int width, int height) {
    try {
        m_projMatrix = glm::perspective(glm::radians(60.0f), (float) width / (float) height, 0.1f, 100.0f);
        m_inputModule = std::make_shared<InputModule>();
        m_cameraModule = std::make_shared<CameraModule>();
    } catch (Exception& error) {
        error.PrintError();
        return false;
    }

    glViewport(0, 0, width, height);

    return true;
}

void GlutDisplay::Destroy() {}

std::shared_ptr<GlutDisplay::InputModule> GlutDisplay::GetInputModule() {
    return m_inputModule;
}

std::shared_ptr<GlutDisplay::CameraModule> GlutDisplay::GetCameraModule() {
    return m_cameraModule;
}
