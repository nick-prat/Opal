#include "display.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl3w.h>

#include <Utilities/exceptions.hpp>
#include <Utilities/log.hpp>
#include <Core/camera.hpp>
#include <Core/inputcontroller.hpp>

Display::Display()
        : m_inputController(nullptr), m_camera(nullptr), m_projMatrix(glm::mat4(1.0f)) {

}

Display::Display(unsigned int width, unsigned int height)
        : m_width(width), m_height(height), m_projMatrix(glm::mat4(1.0f)) {

    m_projMatrix = glm::perspective(glm::radians(60.0f), (float) width / (float) height, 0.1f, 100.0f);
    m_inputController = std::make_unique<InputController>();
    m_camera = std::make_unique<Camera>();
    glViewport(0, 0, width, height);
}

Display::~Display() {

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
