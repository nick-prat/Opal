#include "glcore.hh"

#include <string>

#include <Scene/scene.hh>
#include <Util/exceptions.hh>
#include <Util/log.hh>
#include <iostream>

// TODO Find a way to list all available scenes (./Resources/Scenes/[These folders are scenes])
// TODO Implement notification system for entities (entity can post lambda to be called on an event)

constexpr unsigned int major = 4;
constexpr unsigned int minor = 5;

Opal::GLCore::GLCore(int width, int height, std::string title)
        : m_display(width, height, major, minor, title) {

    if(gl3wInit() == -1) {
        throw GenericException("Couldn't initialize GL3W\n");
    }

    if(!gl3wIsSupported(major, minor)) {
        Log::getErrorLog<SyncLogger>() << "Open GL " << major << "." << minor << " is unsupported\n";
        exit(-1);
    }

    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id,
            GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        auto errorLog = [severity] {
            switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
            case GL_DEBUG_SEVERITY_MEDIUM:
            case GL_DEBUG_SEVERITY_LOW:
                return Log::getErrorLog<SyncLogger>();
            case GL_DEBUG_SEVERITY_NOTIFICATION:
            default:
                return Log::getLog<SyncLogger>();
            }
        }();

        switch (source) {
        case GL_DEBUG_SOURCE_API:
            errorLog << "[API] ";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            errorLog << "[APPLICATION] ";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            errorLog << "[OTHER] ";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            errorLog << "[SHADER_COMPILER] ";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            errorLog << "[THIRD_PARTY] ";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            errorLog << "[WINDOW_SYSTEM] ";
            break;
        default:
            errorLog << "[UNKNOWN] ";
        }

        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            errorLog << "[HIGH] ";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            errorLog << "[MEDIUM] ";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            errorLog << "[LOW] ";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            errorLog << "[NOTIFICATION] ";
            break;
        default:
            errorLog << "[UNKNOWN] ";
            break;
        }

        switch (type) {
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            errorLog << "[DEPRECATED] ";
            break;
        case GL_DEBUG_TYPE_ERROR:
            errorLog << "[ERROR] ";
            break;
        case GL_DEBUG_TYPE_MARKER:
            errorLog << "[MARKER] ";
            break;
        case GL_DEBUG_TYPE_OTHER:
            errorLog << "[OTHER] ";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            errorLog << "[PERFORMANCE] ";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            errorLog << "[POP_GROUP] ";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            errorLog << "[PORTABILITY] ";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            errorLog << "[PUSH_GROUP] ";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            errorLog << "[UNDEFINED] ";
        default:
            errorLog << "[UNKNOWN] ";
            break;
        }

        errorLog << message << '\n';
    }, nullptr);

    // NOTE You can set input mode with this
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Log information about this context
    auto log = Log::getLog<SyncLogger>();
    log << "\nInformation: \n";
    log << "\tGL Version: " << glGetString(GL_VERSION) << '\n';
    log << "\tGLCore Address: " << this << '\n';
    log << "\tDisplay Address: " << &m_display << '\n' << '\n';

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    log << "GL Context created\n";
}

Opal::GLCore::GLCore(GLCore &&glCore)
        : m_display(std::move(glCore.m_display))
        , m_scene(std::move(glCore.m_scene)) {}

Opal::GLCore &Opal::GLCore::operator=(GLCore &&glCore) {
    m_scene = std::move(glCore.m_scene);
    m_display = std::move(glCore.m_display);
    return *this;
}

void Opal::GLCore::start() {
    m_scene->start();
    while(!m_display.windowShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(m_scene.get() != nullptr) {
            m_scene->gameLoop();
        }
        m_display.update();
    }
}

Opal::Display &Opal::GLCore::getDisplay() {
    return m_display;
}

const Opal::Display &Opal::GLCore::getDisplay() const {
    return m_display;
}
