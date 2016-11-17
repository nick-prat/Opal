#include "glcore.hpp"

#include <GL/freeglut.h>
#include <chrono>
#include <string>
#include <thread>
#include <fstream>

#include <Utilities/exceptions.hpp>
#include <Utilities/utilities.hpp>
#include <Utilities/log.hpp>
#include <Resources/resourceloader.hpp>

using namespace gl;

GLCore* GLCore::m_glContext = nullptr;

// Static Functions

void GLCore::DeleteInstance() {
    delete m_glContext;
    m_glContext = nullptr;
}

bool GLCore::CreateInstance(int width, int height, std::string scene) {
    if(m_glContext != nullptr) {
        std::cout << "GL Context has already been created, destroy first";
        return false;
    }

    try {
        m_glContext = new GLCore(width, height, scene);
    } catch (generic_exception& error) {
        error.PrintError();
        delete m_glContext;
        m_glContext = nullptr;
        return false;
    }

    return true;
}

GLCore*& GLCore::GetInstance() {
    return m_glContext;
}

// Non Static Functions

GLCore::GLCore(int width, int height, std::string scene)
        : m_display(nullptr) {

    // Look up all GL functions for later use
    InitAPI();

    // Create standard display with screen dimensions
    m_display = std::make_shared<GlutDisplay>(width, height);

    // Create singleton instance of RenderChain
    if(!RenderChain::CreateInstance(m_display, false)) {
        throw generic_exception("Couldn't Create Instance of RenderChain");
    }

    // Log information about current context
    std::cout << std::endl;
    std::cout << "Information: " << std::endl;
    std::cout << "\tGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "\tDisplay Address: " << m_display << std::endl;
    std::cout << "\tRender Chain Address: " << RenderChain::GetInstance() << std::endl;
    std::cout << std::endl;

    LoadScene(scene);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.1f, 0.0f, 0.0f);

    Log::info("GL Context created", Log::OUT_LOG);
}

GLCore::~GLCore() {
    RenderChain::DeleteInstance();
}

void GLCore::KeyboardFunc(unsigned char key, bool state, int x, int y) {
    m_display->GetInputController()->UpdateKey(key, state);
}

void GLCore::DisplayFunc() {
    auto start = std::chrono::high_resolution_clock::now();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto renderChain = RenderChain::GetInstance();

    if(renderChain != nullptr) {
        renderChain->RenderObjectChain();
    } else {
        Log::error("Render chain was null in render loop", Log::OUT_CONS);
    }

    Utilities::PrintGLErrors();

    glutSwapBuffers();
    auto finish = std::chrono::high_resolution_clock::now();

    std::shared_ptr<InputController> inputController = m_display->GetInputController();

    if(inputController->IsKeyPressed('f')) {
        m_display->SetMousePosition(0.5f, 0.5f);
    }

    if(m_display->GetInputController()->IsKeyPressed('q')) {
        m_display->GetCamera()->MoveCamera(glm::vec3(0.0f, -0.1f, 0.0f));
    }
    if(m_display->GetInputController()->IsKeyPressed('e')) {
        m_display->GetCamera()->MoveCamera(glm::vec3(0.0f, 0.1f, 0.0f));
    }
    if(m_display->GetInputController()->IsKeyPressed('w')) {
        m_display->GetCamera()->MoveCamera(glm::vec3(0.0f, 0.0f, -0.1f));
    }
    if(m_display->GetInputController()->IsKeyPressed('s')) {
        m_display->GetCamera()->MoveCamera(glm::vec3(0.0f, 0.0f, 0.1f));
    }
    if(m_display->GetInputController()->IsKeyPressed('a')) {
        m_display->GetCamera()->MoveCamera(glm::vec3(-0.1f, 0.0f, 0.0f));
    }
    if(m_display->GetInputController()->IsKeyPressed('d')) {
        m_display->GetCamera()->MoveCamera(glm::vec3(0.1f, 0.0f, 0.0f));
    }

    if(m_display->GetInputController()->IsKeyPressed(' ')) {
        std::cout << "Frame Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << std::endl;
    }
}

void GLCore::LoadScene(std::string name) {
    m_renderObjects = ResourceLoader::LoadScene(name);
    for(auto obj : m_renderObjects) {
        RenderChain::GetInstance()->AttachRenderObject(obj);
    }
}
