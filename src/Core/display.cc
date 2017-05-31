#include "display.hh"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl3w.h>

#include <Utilities/exceptions.hh>
#include <Utilities/log.hh>
#include <Core/camera.hh>
#include <Core/inputcontroller.hh>

Display::Display()
        : m_inputController(nullptr), m_camera(nullptr), m_projMatrix({1.0f}) {
}

Display::Display(unsigned int width, unsigned int height)
        : m_width(width), m_height(height), m_projMatrix(glm::mat4(1.0f)) {

    m_projMatrix = glm::perspective(glm::radians(60.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
    m_inputController = std::make_unique<InputController>();
    m_camera = std::make_unique<Camera>();
    glViewport(0, 0, width, height);
}

Display::Display(Display&& display) {
    *this = std::move(display);
}

Display::~Display() {

}

Display& Display::operator=(Display&& display) {
    m_width = display.m_width;
    m_height = display.m_height;
    m_inputController = std::move(display.m_inputController);
    m_camera = std::move(display.m_camera);
    m_projMatrix = std::move(display.m_projMatrix);

    display.m_width = 0;
    display.m_height = 0;

    return *this;
}

InputController* Display::getInputController() const {
    return m_inputController.get();
}

Camera* Display::getCamera() const {
    return m_camera.get();
}

glm::mat4 Display::getProjectionMatrix() const {
    return m_projMatrix;
}

unsigned int Display::getWidth() const {
    return m_width;
}

unsigned int Display::getHeight() const {
    return m_height;
}

// TODO Implement set cursor position
void Display::setCursorPosition(float x, float y) const {
    glm::clamp(x, 0.0f, 1.0f);
    glm::clamp(y, 0.0f, 1.0f);
}

// TODO Implement set cursor visible
void Display::setCursorVisible(bool visible) const {

}

void Display::setWireFrame(bool wireframe) const {
    if(wireframe) {
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
