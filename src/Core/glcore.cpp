#include <GL/gl3w.h>
#include "glcore.hpp"

#include <string>

#include <Scene/scene.hpp>
#include <Utilities/exceptions.hpp>
#include <Utilities/log.hpp>

// TODO Find a way to list all available scenes (./Resources/Scenes/[These folders are scenes])

GLCore::GLCore(int width, int height, std::string title) {

    if(!glfwInit()) {
        Log::error("Couldn't initialize GLFW3\n");
        exit(-1);
    }

    constexpr int major = 3;
    constexpr int minor = 3;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(!m_window) {
        Log::error("Couldn't create window\n");
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(m_window);

    if(gl3wInit() == -1) {
        Log::error("Couldn't initialize GL3W\n");
        glfwTerminate();
        exit(-1);
    }

    if(!gl3wIsSupported(major, minor)) {
        Log::getErrorLog() << "Open GL " << major << "." << minor << " is unsupported\n";
        glfwTerminate();
        exit(-1);
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSwapInterval(1);

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
        GLCore* glCore = reinterpret_cast<GLCore*>(glfwGetWindowUserPointer(window));
        if(action == GLFW_PRESS) {
            glCore->inputFunc(key, true);
        } else if(action == GLFW_RELEASE) {
            glCore->inputFunc(key, false);
        }
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int /*mods*/) {
        GLCore* glCore = reinterpret_cast<GLCore*>(glfwGetWindowUserPointer(window));
        if(action == GLFW_PRESS) {
            glCore->inputFunc(button, true);
        } else if(action == GLFW_RELEASE) {
            glCore->inputFunc(button, false);
        }
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
        GLCore* glCore = reinterpret_cast<GLCore*>(glfwGetWindowUserPointer(window));
        glCore->mouseFunc(xpos, ypos);
    });

    m_display = std::make_unique<const Display>(width, height);

    // Log information about current context
    Log::getLog() << "\nInformation: \n";
    Log::getLog() << "\tGL Version: " << glGetString(GL_VERSION) << '\n';
    Log::getLog() << "\tDisplay Address: " << m_display.get() << "\n\n";

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    Log::getLog() << Log::OUT_LOG << "GL Context created\n";
    Log::getLog() << Log::OUT_LOG_CONS;
}

GLCore::~GLCore() {
    closeScene();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

GLFWwindow* GLCore::getWindow() const {
    return m_window;
}

void GLCore::startScene(const std::string& scene) {
    initScene(scene);
    m_scene->start();
}

void GLCore::displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_scene->gameLoop();
    m_display->getInputController()->callKeyLambdas();
}

void GLCore::inputFunc(int key, bool state) {
    m_display->getInputController()->updateKey(key, state);
}

void GLCore::mouseFunc(double xpos, double ypos) {
    m_display->getInputController()->updateMousePosition(xpos, ypos);
}

void GLCore::initScene(std::string scene) {
    m_luaState = luaL_newstate();
    luaL_openlibs(m_luaState);
    m_scene = std::make_unique<Scene>(m_display.get(), m_luaState, scene);
}

void GLCore::closeScene() {
    m_scene.reset(nullptr);
    lua_close(m_luaState);
}
