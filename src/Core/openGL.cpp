#include "openGL.hpp"

#include <chrono>
#include <string>
#include <thread>
#include <fstream>

#include <Utilities/exceptions.hpp>
#include <Utilities/utilities.hpp>
#include <Utilities/log.hpp>
#include <Resources/resourceloader.hpp>

using namespace gl;

OpenGL* OpenGL::m_openGL = nullptr;

// Static Functions

void OpenGL::DeleteInstance() {
    delete m_openGL;
    m_openGL = nullptr;
}

bool OpenGL::CreateInstance(int width, int height, std::string scene) {
    if(m_openGL != nullptr) {
        std::cout << "OpenGL has already been created, destroy first";
        return false;
    }

    try {
        m_openGL = new OpenGL(width, height, scene);
    } catch (generic_exception& error) {
        error.PrintError();
        delete m_openGL;
        m_openGL = nullptr;
        return false;
    }

    return true;
}

OpenGL*& OpenGL::GetInstance() {
    return m_openGL;
}

// Non Static Functions

OpenGL::OpenGL(int width, int height, std::string scene)
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

    Log::info("OpenGL context created", Log::OUT_LOG);
}

OpenGL::~OpenGL() {
    RenderChain::DeleteInstance();
}

void OpenGL::KeyboardFunc(unsigned char key, bool state, int x, int y) {
    m_display->GetInputModule()->UpdateKey(key, state);
}

void OpenGL::DisplayFunc() {
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

    if(m_display->GetInputModule()->IsKeyPressed('q')) {
        m_display->GetCameraModule()->MoveCamera(glm::vec3(0.0f, 0.1f, 0.0f));
    }
    if(m_display->GetInputModule()->IsKeyPressed('e')) {
        m_display->GetCameraModule()->MoveCamera(glm::vec3(0.0f, -0.1f, 0.0f));
    }
    if(m_display->GetInputModule()->IsKeyPressed('w')) {
        m_display->GetCameraModule()->MoveCamera(glm::vec3(0.0f, 0.0f, 0.1f));
    }
    if(m_display->GetInputModule()->IsKeyPressed('s')) {
        m_display->GetCameraModule()->MoveCamera(glm::vec3(0.0f, 0.0f, -0.1f));
    }
    if(m_display->GetInputModule()->IsKeyPressed('a')) {
        m_display->GetCameraModule()->MoveCamera(glm::vec3(0.1f, 0.0f, 0.0f));
    }
    if(m_display->GetInputModule()->IsKeyPressed('d')) {
        m_display->GetCameraModule()->MoveCamera(glm::vec3(-0.1f, 0.0f, 0.0f));
    }

    if(m_display->GetInputModule()->IsKeyPressed(' ')) {
        std::cout << "Frame Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << std::endl;
    }
}

void OpenGL::LoadScene(std::string name) {
    m_renderObjects = ResourceLoader::LoadScene(name);
    for(auto obj : m_renderObjects) {
        RenderChain::GetInstance()->AttachRenderObject(obj);
    }
}
