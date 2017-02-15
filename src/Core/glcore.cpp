#include <GL/gl3w.h>
#include "glcore.hpp"

#include <string>

#include <Scene/scene.hpp>
#include <Utilities/exceptions.hpp>
#include <Utilities/log.hpp>

// TODO Find a way to list all available scenes (./Resources/Scenes/[These folders are scenes])
// TODO Implement notification system for entities (entity can post lambda to be called on an event)

// Creates a dummy GLCore that doesn't spawn a window
GLCore::GLCore() : m_display(), m_currentScene(nullptr), m_window(nullptr) {
}

GLCore::GLCore(int width, int height, std::string title)
        : m_display(), m_currentScene(nullptr), m_window(nullptr) {
    glfwSetErrorCallback([](int error, const char* desc) {
        Log::getErrorLog() << "ERROR: " << "(" << error << ")" << " " << desc << '\n';
    });

    constexpr int major = 3;
    constexpr int minor = 3;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(!m_window) {
        throw GenericException("Couldn't create window\n");
    }

    glfwMakeContextCurrent(m_window);

    if(gl3wInit() == -1) {
        throw GenericException("Couldn't initialize GL3W\n");
    }

    if(!gl3wIsSupported(major, minor)) {
        Log::getErrorLog() << "Open GL " << major << "." << minor << " is unsupported\n";
        glfwTerminate();
        exit(-1);
    }

    m_display = Display(width, height);

    glfwSetWindowUserPointer(m_window, this);

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
        GLCore* glCore = reinterpret_cast<GLCore*>(glfwGetWindowUserPointer(window));
        if(glCore == nullptr) {
            return;
        }
        if(action == GLFW_PRESS) {
            glCore->inputFunc(key, true);
        } else if(action == GLFW_RELEASE) {
            glCore->inputFunc(key, false);
        }
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int /*mods*/) {
        GLCore* glCore = reinterpret_cast<GLCore*>(glfwGetWindowUserPointer(window));
        if(glCore == nullptr) {
            return;
        }
        if(action == GLFW_PRESS) {
            glCore->inputFunc(button, true);
        } else if(action == GLFW_RELEASE) {
            glCore->inputFunc(button, false);
        }
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
        GLCore* glCore = reinterpret_cast<GLCore*>(glfwGetWindowUserPointer(window));
        if(glCore == nullptr) {
            return;
        }
        glCore->mouseFunc(xpos, ypos);
    });

    // NOTE You can set input mode with this
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Log information about this context
    Log::getLog() << "\nInformation: \n";
    Log::getLog() << "\tGL Version: " << glGetString(GL_VERSION) << '\n';
    Log::getLog() << "\tGLCore Address: " << this << '\n';
    Log::getLog() << "\tDisplay Address: " << &m_display << "\n\n";

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    Log::getLog() << Log::OUT_LOG << "GL Context created\n" << Log::OUT_LOG_CONS;
}

GLCore::GLCore(GLCore&& glCore)
        : m_display(std::move(glCore.m_display))
        , m_currentScene(glCore.m_currentScene)
        , m_window(glCore.m_window) {
    glCore.m_currentScene = nullptr;
    glCore.m_window = nullptr;

    if(m_window != nullptr) {
        glfwSetWindowUserPointer(m_window, this);
    }
}

GLCore::~GLCore() {
    destroy();
}

GLCore& GLCore::operator=(GLCore&& glCore) {
    m_display = std::move(glCore.m_display);
    m_currentScene = glCore.m_currentScene;
    m_window = glCore.m_window;

    glCore.m_currentScene = nullptr;
    glCore.m_window = nullptr;

    if(m_window != nullptr) {
        glfwSetWindowUserPointer(m_window, this);
    }

    return *this;
}

void GLCore::makeWindowCurrent(GLCore* glCore) {

}

void GLCore::initAPI() {
    if(!glfwInit()) {
        throw GenericException("Couldn't initialize GLFW3\n");
    }
}

void GLCore::closeAPI() {
    glfwTerminate();
}

void GLCore::destroy() {
    if(m_window != nullptr) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

bool GLCore::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void GLCore::setClearColor(const glm::vec4& color) {
    glClearColor(color.x, color.y, color.z, color.w);
}

void GLCore::setVsync(bool enabled) {
    if(enabled) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
}

GLFWwindow* GLCore::getWindow() const {
    return m_window;
}

const Display& GLCore::getDisplay() const {
    return m_display;
}

Scene GLCore::createScene(const std::string& scenename) {
    auto timer = glfwGetTime();
    auto scene = Scene(&m_display, scenename);
    Log::getLog() << "Scene creation for " << scenename << " in " << glfwGetTime() - timer << " seconds\n";
    return scene;
}

void GLCore::startScene(Scene* scene) {
    m_currentScene = scene;
    m_currentScene->start();
}

void GLCore::displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(m_currentScene != nullptr) {
        m_currentScene->gameLoop();
    }
    m_display.getInputController()->callKeyLambdas();
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void GLCore::inputFunc(int key, bool state) {
    m_display.getInputController()->updateKey(key, state);
}

void GLCore::mouseFunc(double xpos, double ypos) {
    m_display.getInputController()->updateMousePosition(xpos, ypos);
}
