#include "openGL.h"

#include <chrono>
#include <string>
#include <thread>
#include <Utilities/utilities.h>
#include <Utilities/log.h>

#include <Model/Assimp/assimploader.h>
#include <Model/Models/staticmodel.h>

OpenGL* OpenGL::m_openGL = nullptr;

void OpenGL::DeleteInstance()
{
    delete m_openGL;
    m_openGL = nullptr;
}

bool OpenGL::CreateInstance(int width, int height, std::string title)
{
    //Log::info("Create OpenGL Instance", true);
    if(m_openGL != nullptr)
    {
        std::cout << "OpenGL has already been created, destroy first";
        return false;
    }

    try
    {
        m_openGL = new OpenGL(width, height, title);
    }
    catch (Utilities::Exception* error)
    {
        error->PrintError();
        delete error;
        delete m_openGL;
        m_openGL = nullptr;
        return false;
    }

    return true;
}

OpenGL*& OpenGL::GetInstance()
{
    return m_openGL;
}

OpenGL::OpenGL(int width, int height, std::string title)
{
    m_lowestTime = 0;

    // Look up all GL functions for later use
    gl::InitAPI();

    // Create singleton instance of RenderChain (Capability of 10 objects)
    if(!RenderChain::CreateInstance(10, false))
    {
        throw new Utilities::Exception(1, "Couldn't Create Instance of RenderChain");
    }

    m_display = std::make_shared<GlutDisplay>(width, height, title);

    m_staticModel = std::make_shared<StaticModel>(m_display, AssimpLoader::LoadModel("Models/wolf.3ds"));
    m_staticModel->Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    m_staticModel->Translate(glm::vec3(0.0f, 0.0f, -10.0f));
    RenderChain::GetInstance()->AttachRenderObject(m_staticModel.get());

    m_obj = std::make_shared<TestObject>(m_display);
    //RenderChain::GetInstance()->AttachRenderObject(m_obj.get());

    m_obj2 = std::make_shared<TestObject>(m_display);
    //RenderChain::GetInstance()->AttachRenderObject(m_obj2.get());

    m_obj->Translate(glm::vec3(0.5f, 0.5f, -0.5f));

    // Log information about current context
    std::cout << "Information: " << std::endl;
    std::cout << "\tGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "\tDisplay Address: " << m_display << std::endl;
    std::cout << "\tRender Chain Address: " << RenderChain::GetInstance() << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    Log::info("OpenGL context created", Log::OUT_LOG);
}

OpenGL::~OpenGL()
{}

void OpenGL::KeyboardFunc(unsigned char key, bool state, int x, int y)
{
    m_display->GetInputModule()->UpdateKey(key, state);
}

void OpenGL::DisplayFunc()
{
    auto start = std::chrono::high_resolution_clock::now();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderChain::GetInstance()->RenderObjectChain();

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
