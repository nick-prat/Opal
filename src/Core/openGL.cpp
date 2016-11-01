#include "openGL.hpp"

#include <chrono>
#include <string>
#include <thread>
#include <fstream>

#include <Utilities/utilities.hpp>
#include <Utilities/log.hpp>
#include <Resources/resourceloader.hpp>

OpenGL* OpenGL::m_openGL = nullptr;

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
    } catch (Utilities::Exception* error) {
        error->PrintError();
        delete error;
        delete m_openGL;
        m_openGL = nullptr;
        return false;
    }

    return true;
}

OpenGL*& OpenGL::GetInstance() {
    return m_openGL;
}

OpenGL::OpenGL(int width, int height)
        : m_lowestTime(0), m_display(nullptr), m_staticModel(nullptr) {

    // Look up all GL functions for later use
    gl::InitAPI();

    // Create singleton instance of RenderChain (Capability of 10 objects)
    if(!RenderChain::CreateInstance(false)) {
        throw new Utilities::Exception(1, "Couldn't Create Instance of RenderChain");
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
        throw new Utilities::Exception(1, "Couldn't open model selection file");
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
    textures["texture"] = ResourceLoader::LoadTexture("bear/bear", false);

    auto model = std::make_shared<Model3D>(meshes, textures);

    //m_staticModel = std::make_shared<StaticModel>(m_display, ResourceLoader::LoadModel3D(line));
    m_staticModel = std::make_shared<StaticModel>(m_display, model);
    m_staticModel->GetModel()->PrintTextures();
    m_staticModel->Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    // TODO only renders one of the lines..
    m_lines.push_back(std::make_shared<Line>(m_display, glm::vec3(-100.0f, 0.0f, 0.0f), glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    m_lines.push_back(std::make_shared<Line>(m_display, glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    //m_lines.push_back(std::make_shared<Line>(m_display, glm::vec3(0.0f, 0.0f, -100.0f), glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

    std::cout << m_lines.size() << std::endl;
    for(std::shared_ptr<Line> line : m_lines) {
        RenderChain::GetInstance()->AttachRenderObject(line);
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
