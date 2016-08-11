//
// Created by nprat on 7/4/16.
//

#include <chrono>
#include <string>
#include <thread>
#include <Model/Assimp/assimploader.h>

#include "openGL.h"

OpenGL* OpenGL::m_openGL = nullptr;

OpenGL::OpenGL()
{
    m_openGL = nullptr;
    m_obj = nullptr;
    m_display = nullptr;
    m_lowestTime = 0;
}

OpenGL::~OpenGL()
{}

bool OpenGL::InitOpenGL(int width, int height, std::string title)
{
    gl::InitAPI();

    // Create singleton instance of RenderChain (Capability of 10 objects)
    if(!RenderChain::CreateInstance(10, true))
    {
        std::cout << "Couldn't Create Instance of RenderChain" << std::endl;
        return -1;
    }

    try
    {
        m_display = std::make_shared<GlutDisplay>(width, height, title);
        m_obj = std::make_shared<TestObject>(m_display);
        m_obj2 = std::make_shared<TestObject>(m_display);
    }
    catch (const char* error)
    {
        std::cout << "Error: " << error << std::endl;
        return false;
    }

    m_obj->Translate(glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, -0.5f)));

    std::cout << "Information: " << std::endl;
    std::cout << "\tGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "\tDisplay Address: " << m_display << std::endl;
    std::cout << "\tRender Chain Address: " << RenderChain::GetInstance() << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    return true;
}

void OpenGL::Destroy()
{}

void OpenGL::KeyboardFunc(unsigned char key, bool state, int x, int y)
{
    m_display->GetInputModule()->UpdateKey(key, state);
}

void OpenGL::DisplayFunc()
{
    auto start = std::chrono::high_resolution_clock::now();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderChain* renderChain = RenderChain::GetInstance();
    renderChain->AttachRenderObject(m_obj.get());
    renderChain->AttachRenderObject(m_obj2.get());
    renderChain->RenderObjectChain();
    glutSwapBuffers();

    auto finish = std::chrono::high_resolution_clock::now();

    if(m_display->GetInputModule()->IsKeyPressed('w'))
    {
        m_display->GetCameraModule()->MoveCamera(glm::vec3(0.0f, 0.0f, 0.1f));
    }
    if(m_display->GetInputModule()->IsKeyPressed('s'))
    {
        m_display->GetCameraModule()->MoveCamera(glm::vec3(0.0f, 0.0f, -0.1f));
    }
    if(m_display->GetInputModule()->IsKeyPressed('a'))
    {
        m_display->GetCameraModule()->MoveCamera(glm::vec3(0.1f, 0.0f, 0.0f));
    }
    if(m_display->GetInputModule()->IsKeyPressed('d'))
    {
        m_display->GetCameraModule()->MoveCamera(glm::vec3(-0.1f, 0.0f, 0.0f));
    }

    if(m_display->GetInputModule()->IsKeyPressed(' '))
    {
        std::cout << "Frame Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << std::endl;
    }
}

void OpenGL::DeleteInstance()
{
    m_openGL->Destroy();
    delete m_openGL;
    m_openGL = nullptr;
}

bool OpenGL::CreateInstance(int width, int height, std::string title)
{
    if(m_openGL != nullptr)
    {
        std::cout << "OpenGL has already been created, destroy first";
        return false;
    }

    m_openGL = new OpenGL();
    if(!m_openGL->InitOpenGL(width, height, title))
    {
        std::cout << "Couldn't initialize OpenGL project" << std::endl;
        return false;
    }

    return true;
}

OpenGL* OpenGL::GetInstance()
{
    return m_openGL;
}
