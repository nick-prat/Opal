#include "glcore.hpp"
#include <GL/gl3w.h>

#include <string>

#include <Scene/scene.hpp>
#include <Utilities/exceptions.hpp>
#include <Utilities/log.hpp>

GLCore::GLCore(int width, int height, std::string scene) {

    m_display = std::make_unique<Display>(width, height);

    // Log information about current context
    Log::getLog() << "\nInformation: \n";
    Log::getLog() << "\tGL Version: " << glGetString(GL_VERSION) << '\n';
    Log::getLog() << "\tDisplay Address: " << m_display.get() << "\n\n";

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.1f, 0.0f, 0.0f);

    Log::getLog() << Log::OUT_LOG << "GL Context created\n";
    Log::getLog() << Log::OUT_LOG_CONS;

    initScene(scene);
    m_scene->start();
}

GLCore::~GLCore() {
    closeScene();
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
