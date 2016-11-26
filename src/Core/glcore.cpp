#include <GL/gl3w.h>

#include "glcore.hpp"

#include <chrono>
#include <string>
#include <thread>
#include <fstream>

#include <Scene/scene.hpp>
#include <Utilities/exceptions.hpp>
#include <Utilities/utilities.hpp>
#include <Utilities/log.hpp>
#include <Resources/resourceloader.hpp>

GLCore::GLCore(int width, int height, std::string scene) {

    m_display = std::make_unique<Display>(width, height);
    m_renderChain = std::make_unique<RenderChain>();

    // Log information about current context
    std::cout << "\nInformation: \n";
    std::cout << "\tGL Version: " << glGetString(GL_VERSION) << '\n';
    std::cout << "\tDisplay Address: " << m_display.get() << '\n';
    std::cout << "\tRender Chain Address: " << m_renderChain.get() << "\n\n";

    m_sceneController = std::make_unique<SceneController>(scene, m_display.get(), m_renderChain.get());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.1f, 0.0f, 0.0f);

    Log::info("GL Context created", Log::OUT_LOG);
}

GLCore::~GLCore() {
}

void GLCore::InputFunc(int key, bool state) {
    m_display->GetInputController()->UpdateKey(key, state);
}

void GLCore::MouseFunc(double xpos, double ypos) {
    m_display->GetInputController()->UpdateMousePosition(xpos, ypos);
}

void GLCore::DisplayFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_sceneController->Render();
    m_renderChain->RenderObjectChain(m_display.get());
    m_display->GetInputController()->CallKeyLambdas();
}
