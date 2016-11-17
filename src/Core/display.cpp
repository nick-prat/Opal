#include "display.hpp"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/freeglut.h>

#include <glapi.hpp>
#include <Utilities/exceptions.hpp>

GlutDisplay::GlutDisplay()
        : m_inputController(nullptr), m_camera(nullptr), m_projMatrix(glm::mat4(1.0f)) {
}

GlutDisplay::GlutDisplay(uint width, uint height)
    : m_projMatrix(glm::mat4(1.0f)) {
    if(!InitDisplay(width, height))
    {
        throw generic_exception("Couldn't init display");
    }
}

GlutDisplay::~GlutDisplay() {
    Destroy();
}

bool GlutDisplay::InitDisplay(uint width, uint height) {

    try {
        m_projMatrix = glm::perspective(glm::radians(60.0f), (float) width / (float) height, 0.1f, 100.0f);
        m_inputController = std::make_shared<InputController>();
        m_camera = std::make_shared<Camera>();
    } catch (generic_exception& error) {
        error.PrintError();
        return false;
    }

    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);

    return true;
}

void GlutDisplay::Destroy() {}

std::shared_ptr<InputController> GlutDisplay::GetInputController() const {
    return m_inputController;
}

std::shared_ptr<Camera> GlutDisplay::GetCamera() const {
    return m_camera;
}

glm::mat4 GlutDisplay::GetProjectionMatrix() const {
    return m_projMatrix;
}

uint GlutDisplay::GetWidth() const {
    return m_width;
}

uint GlutDisplay::GetHeight() const {
    return m_height;
}

void GlutDisplay::SetMousePosition(float x, float y) const {
    glm::clamp(x, 0.0f, 1.0f);
    glm::clamp(y, 0.0f, 1.0f);
    glutWarpPointer((int)(x * m_width), (int)(y * m_height));
}
