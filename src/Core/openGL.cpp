#include "openGL.hpp"

#include <chrono>
#include <string>
#include <thread>
#include <fstream>

#include <Utilities/utilities.hpp>
#include <Utilities/log.hpp>
#include <Resources/resourceloader.hpp>

using namespace gl;
using Utilities::Exception;

OpenGL* OpenGL::m_openGL = nullptr;

// Static Functions

void OpenGL::DeleteInstance() {
    delete m_openGL;
    m_openGL = nullptr;
}

bool OpenGL::CreateInstance(int width, int height) {
    if(m_openGL != nullptr) {
        std::cout << "OpenGL has already been created, destroy first";
        return false;
    }

    try {
        m_openGL = new OpenGL(width, height);
    } catch (Exception& error) {
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

OpenGL::OpenGL(int width, int height)
        : m_display(nullptr) {

    // Look up all GL functions for later use
    InitAPI();

    // Create singleton instance of RenderChain (Capability of 10 objects)
    if(!RenderChain::CreateInstance(false)) {
        throw Exception("Couldn't Create Instance of RenderChain");
    }

    m_display = std::make_shared<GlutDisplay>(width, height);

    // Log information about current context
    std::cout << "Information: " << std::endl;
    std::cout << "\tGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "\tDisplay Address: " << m_display << std::endl;
    std::cout << "\tRender Chain Address: " << RenderChain::GetInstance() << std::endl;

    std::string line;
    std::ifstream file("model.txt");

    if(file.is_open()) {
        getline(file, line);
    } else {
        throw Exception("Couldn't open model selection file");
    }

    m_renderObjects = ResourceLoader::LoadScene(m_display, "defscene.json");
    for(auto obj : m_renderObjects) {
        RenderChain::GetInstance()->AttachRenderObject(obj);
    }

    std::vector<Model3D::Vertex> verts;
    verts.push_back(Model3D::Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
    verts.push_back(Model3D::Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)));
    verts.push_back(Model3D::Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)));
    verts.push_back(Model3D::Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)));

    std::vector<GLuint> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(1);

    std::vector<std::shared_ptr<Model3D::Mesh>> meshes;
    meshes.push_back(std::make_shared<Model3D::Mesh>(verts, indices));
    meshes[0]->SetMatName("texture");

    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
    textures["texture"] = ResourceLoader::LoadTexture("wolf/wolf", false);

    auto model = std::make_shared<Model3D>(meshes, textures);

    //m_staticModel = std::make_shared<StaticModel>(m_display, ResourceLoader::LoadModel3D(line));
    m_staticModels.push_back(std::make_shared<StaticModel>(m_display, model));
    m_staticModels[0]->GetModel()->PrintTextures();
    m_staticModels[0]->Translate(glm::vec3(-2.5f, 0.0f, 0.0f));
    m_staticModels[0]->Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    m_staticModels.push_back(std::make_shared<StaticModel>(m_display, model));
    m_staticModels[1]->Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    for(std::shared_ptr<StaticModel> model : m_staticModels) {
        RenderChain::GetInstance()->AttachRenderObject(model);
    }

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

bool OpenGL::LoadScene(std::string name) {
    return true;
}
