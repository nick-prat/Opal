//
// Created by nprat on 7/4/16.
//

#include <GL/glew.h>
#include <GL/gl.h>
#include <chrono>
#include <string>

#include "OpenGL.h"

std::shared_ptr<OpenGL> OpenGL::m_openGL = nullptr;

OpenGL::OpenGL()
{
    m_openGL = nullptr;
    m_renderChain = nullptr;
    m_shittyObject = nullptr;
    m_display = nullptr;
    m_lowestTime = 0;
}

OpenGL::~OpenGL()
{}

bool OpenGL::InitOpenGL(int width, int height, std::string title)
{
    gl::InitAPI();
    // TODO replace GLEW with gl function look ups
    /*glewExperimental = GL_TRUE;
    GLenum status = glewInit();
    std::cout << "Error " << gluErrorString(glGetError()) << std::endl;
    if(status != GLEW_OK)
    {
        std::cout << "Glew failed to initialize: " << status << std::endl;
        return false;
    }*/

    try
    {
        m_display = std::make_shared<Display>(width, height, title);
        m_renderChain = std::make_shared<RenderChain>(10);
        m_shittyObject = std::make_shared<ShittyObject>(m_display);
    }
    catch (const char* error)
    {
        std::cout << "Error: " << error << std::endl;
        return false;
    }

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

    return true;
}

void OpenGL::Destroy()
{

}

void OpenGL::DisplayFunc()
{
    auto start = std::chrono::high_resolution_clock::now();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_renderChain->AttachRenderObject(m_shittyObject.get());
    m_renderChain->RenderObjectChain();

    glutSwapBuffers();

    auto finish = std::chrono::high_resolution_clock::now();
    
    /*if(time < m_lowestTime || m_lowestTime == 0)
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

