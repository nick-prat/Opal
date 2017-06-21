#include "display.hh"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Utilities/exceptions.hh>
#include <Utilities/log.hh>
#include <Core/camera.hh>
#include <Core/inputcontroller.hh>

Display::Display()
        : m_inputController(nullptr)
        , m_camera(nullptr)
        , m_projMatrix({1.0f}) {
}

Display::Display(unsigned int width, unsigned int height, unsigned int major, unsigned int minor, const std::string& title)
        : m_width(width)
        , m_height(height)
        , m_projMatrix(glm::mat4(1.0f)) {

    if(!glfwInit()) {
        throw GenericException("Couldn't initialize GLFW3\n");
    }

    glfwSetErrorCallback([](int error, const char* desc) {
        Log::getErrorLog() << "ERROR: " << "(" << error << ")" << " " << desc << '\n';
    });

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(!m_window) {
        throw GenericException("Couldn't create window\n");
    }

    glfwMakeContextCurrent(m_window);

    glfwSetWindowUserPointer(m_window, this);

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
        Display* display = reinterpret_cast<Display*>(glfwGetWindowUserPointer(window));
        if(display == nullptr) {
            return;
        }

        if(action == GLFW_PRESS) {
            display->getInputController()->updateKey(key, true);
        } else if(action == GLFW_RELEASE) {
            display->getInputController()->updateKey(key, false);
        }
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int /*mods*/) {
        Display* display = reinterpret_cast<Display*>(glfwGetWindowUserPointer(window));
        if(display == nullptr) {
            return;
        }
        if(action == GLFW_PRESS) {
            display->getInputController()->updateKey(button, true);
        } else if(action == GLFW_RELEASE) {
            display->getInputController()->updateKey(button, false);
        }
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
        Display* display = reinterpret_cast<Display*>(glfwGetWindowUserPointer(window));
        if(display == nullptr) {
            return;
        }
        display->getInputController()->updateMousePosition(xpos, ypos);
    });

    m_projMatrix = glm::perspective(glm::radians(60.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
    m_inputController = std::make_unique<InputController>();
    m_camera = std::make_unique<Camera>();
}

Display::Display(Display&& display)
        : m_window(display.m_window)
        , m_width(display.m_width)
        , m_height(display.m_height)
        , m_inputController(std::move(display.m_inputController))
        , m_camera(std::move(display.m_camera))
        , m_projMatrix(std::move(display.m_projMatrix)) {
    display.m_window = nullptr;
    display.m_width = 0;
    display.m_height = 0;

    glfwTerminate();
}

Display::~Display() {
    glfwDestroyWindow(m_window);
}

Display& Display::operator=(Display&& display) {
    m_window = display.m_window;
    m_width = display.m_width;
    m_height = display.m_height;
    m_inputController = std::move(display.m_inputController);
    m_camera = std::move(display.m_camera);
    m_projMatrix = std::move(display.m_projMatrix);

    display.m_window = nullptr;
    display.m_width = 0;
    display.m_height = 0;

    if(m_window != nullptr) {
        glfwSetWindowUserPointer(m_window, this);
    }

    return *this;
}

void Display::resize() {
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);
}

void Display::update() {
    getInputController()->callKeyLambdas();
    glfwSwapBuffers(m_window);
    glfwPollEvents();
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

bool Display::windowShouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Display::setMouseCapture(bool capture) {
    if(capture) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Display::setClearColor(const glm::vec4& color) {
    glClearColor(color.x, color.y, color.z, color.w);
}

void Display::setVsync(bool enabled) {
    if(enabled) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
}

// TODO Implement set cursor position
void Display::setCursorPosition(float x, float y) {
    glm::clamp(x, 0.0f, 1.0f);
    glm::clamp(y, 0.0f, 1.0f);
}

// TODO Implement set cursor visible
void Display::setCursorVisible(bool visible) {

}

void Display::setWireFrame(bool wireframe) {
    if(wireframe) {
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
