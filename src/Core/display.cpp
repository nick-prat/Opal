#include "display.hpp"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glapi.hpp>
#include <Utilities/exceptions.hpp>

GlutDisplay::GlutDisplay()
        : m_inputController(nullptr), m_camera(nullptr), m_projMatrix(glm::mat4(1.0f)) {
}

GlutDisplay::GlutDisplay(int width, int height)
    : m_projMatrix(glm::mat4(1.0f)) {
    if(!InitDisplay(width, height))
    {
        throw generic_exception("Couldn't init display");
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
        m_inputController = std::make_shared<InputController>();
        m_camera = std::make_shared<Camera>();
    } catch (generic_exception& error) {
        error.PrintError();
        return false;
    }

    glViewport(0, 0, width, height);

    return true;
}

void GlutDisplay::Destroy() {}

std::shared_ptr<InputController> GlutDisplay::GetInputController() {
    return m_inputController;
}

std::shared_ptr<Camera> GlutDisplay::GetCamera() {
    return m_camera;
}
