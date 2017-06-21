#include "glcore.hh"

#include <string>

#include <Scene/scene.hh>
#include <Utilities/exceptions.hh>
#include <Utilities/log.hh>

// TODO Find a way to list all available scenes (./Resources/Scenes/[These folders are scenes])
// TODO Implement notification system for entities (entity can post lambda to be called on an event)

constexpr unsigned int major = 4;
constexpr unsigned int minor = 5;

// Creates a dummy GLCore that doesn't spawn a window
GLCore::GLCore()
        : m_display() {}

GLCore::GLCore(int width, int height, std::string title)
        : m_display(width, height, major, minor, title) {

    if(gl3wInit() == -1) {
        throw GenericException("Couldn't initialize GL3W\n");
    }

    if(!gl3wIsSupported(major, minor)) {
        Log::getErrorLog() << "Open GL " << major << "." << minor << " is unsupported\n";
        exit(-1);
    }

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
        , m_scene(std::move(glCore.m_scene)) {}

GLCore::~GLCore() {}

GLCore& GLCore::operator=(GLCore&& glCore) {
    m_scene = std::move(glCore.m_scene);
    m_display = std::move(glCore.m_display);
    return *this;
}

void GLCore::start() {
    m_scene->start();

    // double timer = glfwGetTime();
    // unsigned long frames = 0;

    while(!m_display.windowShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(m_scene.get() != nullptr) {
            m_scene->gameLoop();
        }
        m_display.update();
        // frames++;
    }

    // timer = glfwGetTime() - timer;
    // Log::getLog() << "Aversage FPS: " << frames / timer << '\n';
}

Display& GLCore::getDisplay() {
    return m_display;
}

const Display& GLCore::getDisplay() const {
    return m_display;
}

void GLCore::loadScene(const std::string& scenename) {
    // auto timer = glfwGetTime();
    m_scene = std::make_unique<Scene>(m_display, scenename);
    // Log::getLog() << "Scene creation for " << scenename << " in " << glfwGetTime() - timer << " seconds\n";
}

void GLCore::inputFunc(int key, bool state) {
    m_display.getInputController()->updateKey(key, state);
}

void GLCore::mouseFunc(double xpos, double ypos) {
    m_display.getInputController()->updateMousePosition(xpos, ypos);
}
