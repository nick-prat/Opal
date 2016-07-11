//
// Created by nprat on 7/4/16.
//

#include <chrono>
#include <string>

#include "openGL.h"

std::shared_ptr<OpenGL> OpenGL::m_openGL = nullptr;

OpenGL::OpenGL()
{
    m_openGL = nullptr;
    m_renderChain = nullptr;
    m_obj = nullptr;
    m_display = nullptr;
    m_lowestTime = 0;
}

OpenGL::~OpenGL()
{}

bool OpenGL::InitOpenGL(int width, int height, std::string title)
{
    std::cout << "Creating API" << std::endl;
    gl::InitAPI();
    std::cout << "Finished Creating API" << std::endl;

    try
    {
        m_display = std::make_shared<GlutDisplay>(width, height, title);
        m_renderChain = std::make_shared<RenderChain>(10);
        m_obj = std::make_shared<TestObject>(m_display);
        m_obj2 = std::make_shared<TestObject>(m_display);
    }
    catch (const char* error)
    {
        std::cout << "Error: " << error << std::endl;
        return false;
    }

    m_obj->Translate(glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, -0.5f)));

    //obj->Scale(glm::scale(glm::vec3(0.5f, 1.0f, 1.0f)));
    //obj->Rotate(glm::rotate(glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)));
    //obj->Translate(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    std::cout << "Information: " << std::endl;
    std::cout << "\tGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "\tDisplay Address: " << m_display << std::endl;
    std::cout << "\tRender Chain Address: " << m_renderChain << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    return true;
}

void OpenGL::Destroy()
{

}

void OpenGL::DisplayFunc()
{
    //std::cout << "DisplayFunc()" << std::endl;
    //auto start = std::chrono::high_resolution_clock::now();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //std::cout << "OpenGL::DisplayFunc()" << std::endl;
    m_renderChain->AttachRenderObject(m_obj.get());
    m_renderChain->AttachRenderObject(m_obj2.get());
    m_renderChain->RenderObjectChain();
    glutSwapBuffers();

    /*auto finish = std::chrono::high_resolution_clock::now();
    
    if(time < m_lowestTime || m_lowestTime == 0)
    {
        std::cout << "Fastest render : " << time << std::endl;
        m_lowestTime = time;
    }*/
    /*
    if(m_display->GetInputModule()->IsKeyPressed(Key_W))
    {
        m_display->GetCameraModule()->MoveCamera(glm::vec3(0.0f, 0.0f, 0.1f));
    }
    if(m_display->GetInputModule()->IsKeyPressed(Key_S))
    {
        m_display->GetCameraModule()->MoveCamera(glm::vec3(0.0f, 0.0f, -0.1f));
    }
    if(m_display->GetInputModule()->IsKeyPressed(Key_A))
    {
        m_display->GetCameraModule()->MoveCamera(glm::vec3(0.1f, 0.0f, 0.0f));
    }
    if(m_display->GetInputModule()->IsKeyPressed(Key_D))
    {
        m_display->GetCameraModule()->MoveCamera(glm::vec3(-0.1f, 0.0f, 0.0f));
    }

    if(m_display->GetInputModule()->IsKeyPressed(Key_Space))
    {
        std::cout << "Frame Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << std::endl;
    }
     */
}

void OpenGL::DestroyInstance()
{
    m_openGL->Destroy();
    m_openGL = nullptr;
}

bool OpenGL::CreateInstance(int width, int height, std::string title)
{
    if(m_openGL != nullptr)
    {
        std::cout << "OpenGL has already been created, destroy first";
        return false;
    }

    m_openGL = std::make_shared<OpenGL>();
    if(!m_openGL->InitOpenGL(width, height, title))
    {
        std::cout << "Couldn't initialize OpenGL project" << std::endl;
        return false;
    }

    return true;
}

std::shared_ptr<OpenGL> OpenGL::getInstance()
{
    return m_openGL;
}

