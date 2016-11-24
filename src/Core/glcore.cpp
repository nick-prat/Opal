#include <GL/gl3w.h>

#include "glcore.hpp"

#include <chrono>
#include <string>
#include <thread>
#include <fstream>

#include <Utilities/exceptions.hpp>
#include <Utilities/utilities.hpp>
#include <Utilities/log.hpp>
#include <Resources/resourceloader.hpp>

GLCore::GLCore(int width, int height, std::string scene)
        : m_renderChain(nullptr), m_display(nullptr) {

    // Create standard display with screen dimensions
    m_display = std::make_shared<Display>(width, height);

    // Create singleton instance of RenderChain
    m_renderChain = std::make_shared<RenderChain>(m_display, false);

    // Log information about current context
    std::cout << std::endl;
    std::cout << "Information: " << std::endl;
    std::cout << "\tGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "\tDisplay Address: " << m_display << std::endl;
    std::cout << "\tRender Chain Address: " << m_renderChain << std::endl;
    std::cout << std::endl;

    LoadScene(scene);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.1f, 0.0f, 0.0f);

    auto inputController = m_display->GetInputController();

    inputController->RegisterWhileKeyPressed(InputKey::A, [this]() {
        m_display->GetCamera()->MoveCamera(glm::vec3(-0.1f, 0.0f, 0.0f));
    });
    inputController->RegisterWhileKeyPressed(InputKey::S, [this]() {
        m_display->GetCamera()->MoveCamera(glm::vec3(0.0f, 0.0f, 0.1f));
    });
    inputController->RegisterWhileKeyPressed(InputKey::D, [this]() {
        m_display->GetCamera()->MoveCamera(glm::vec3(0.1f, 0.0f, 0.0f));
    });
    inputController->RegisterWhileKeyPressed(InputKey::W, [this]() {
        m_display->GetCamera()->MoveCamera(glm::vec3(0.0f, 0.0f, -0.1f));
    });
    inputController->RegisterWhileKeyPressed(InputKey::Q, [this]() {
        m_display->GetCamera()->MoveCamera(glm::vec3(0.0f, -0.1f, 0.0f));
    });
    inputController->RegisterWhileKeyPressed(InputKey::E, [this]() {
        m_display->GetCamera()->MoveCamera(glm::vec3(0.0f, 0.1f, 0.0f));
    });

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
    auto start = std::chrono::high_resolution_clock::now();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_renderChain->RenderObjectChain();
    Utilities::PrintGLErrors();

    auto finish = std::chrono::high_resolution_clock::now();

    std::shared_ptr<InputController> inputController = m_display->GetInputController();
    if(inputController->IsKeyPressed(InputKey::SPACE)) {
        std::cout << "Frame Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << std::endl;
    }

    inputController->CallKeyLambdas();
}

void GLCore::LoadScene(std::string name) {
    m_renderObjects = ResourceLoader::LoadScene(name);
    for(auto obj : m_renderObjects) {
        m_renderChain->AttachRenderObject(obj);
    }
}
