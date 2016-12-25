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
#include <Resources/model3d.hpp>

using json = nlohmann::json;

GLCore::GLCore(int width, int height, std::string scene) {

    m_display = std::make_unique<Display>(width, height);
    m_renderChain = std::make_unique<RenderChain>();
    m_resourceHandler = std::make_unique<ResourceHandler>();

    // Log information about current context
    Log::getLog() << "\nInformation: \n";
    Log::getLog() << "\tGL Version: " << glGetString(GL_VERSION) << '\n';
    Log::getLog() << "\tDisplay Address: " << m_display.get() << '\n';
    Log::getLog() << "\tRender Chain Address: " << m_renderChain.get() << "\n\n";

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.1f, 0.0f, 0.0f);

    Log::getLog() << Log::OUT_LOG << "GL Context created" << Log::OUT_LOG_CONS;

    initScene(scene);

    for(const auto& object : m_staticModels) {
        m_renderChain->attach(object.get());
    }

    for(const auto& object : m_dynamicModels) {
        m_renderChain->attach(object.second.get());
    }

    Log::getLog() << "\nRenderObject count: " << IRenderObject::getNumRenderObjects() << '\n';
    m_scene->start();
}

GLCore::~GLCore() {
    closeScene();
}

void GLCore::displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_renderChain->render(m_display.get());
    m_scene->gameLoop();
    m_display->GetInputController()->CallKeyLambdas();
}

void GLCore::inputFunc(int key, bool state) {
    m_display->GetInputController()->UpdateKey(key, state);
}

void GLCore::mouseFunc(double xpos, double ypos) {
    m_display->GetInputController()->UpdateMousePosition(xpos, ypos);
}

void GLCore::initScene(std::string scene) {

    m_luaState = luaL_newstate();
    luaL_openlibs(m_luaState);
    m_scene = std::make_unique<Scene>(m_display.get(), m_luaState, m_resourceHandler.get(), scene);

    std::string filename = "Resources/Scenes/" + scene + "/scene.json";
    std::string contents;
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
    } else {
        throw GenericException(filename + " doesn't exist");
    }

    try {
        json scene = json::parse(contents);

        if(scene.find("resources") != scene.end()) {
            std::vector<json> resources = scene["resources"];
            for(json resource : resources) {
                std::string type = resource["type"];
                std::string name = resource["resourcename"];
                std::string filename = resource["filename"];

                if(type == "model3d") {
                    m_resourceHandler->AddResource(name, m_resourceHandler->LoadModel3D(filename));
                }
            }
        }

        if(scene.find("staticObjects") != scene.end()) {
            std::vector<json> objects = scene["staticObjects"];
            for(json object : objects) {
                try {
                    std::string type = object["type"];
                    IRenderObject* rObject = nullptr;

                    if(type == "line") {
                        rObject = m_resourceHandler->LoadLineJSON(object);
                    } else if(type == "staticmodel") {
                        rObject = m_resourceHandler->GenerateModel(object, m_resourceHandler->GetResource<Model3D>(object["resource"]));
                    } else if(type == "rawstaticmodel") {
                        rObject = m_resourceHandler->GenerateModel(object);
                    }

                    if(rObject != nullptr) {
                        m_staticModels.push_back(std::unique_ptr<IRenderObject>(rObject));
                    }
                } catch (BadResource& error) {
                    error.printError();
                } catch (std::domain_error& error) {
                    std::cout << error.what() << '\n';
                }
            }
        }

        // TODO Implement actual dynamic model loading
        if(scene.find("dynamicObjects") != scene.end()) {
            std::vector<json> objects = scene["dynamicObjects"];
            for(json object : objects) {
                try {
                    auto name = object["name"];
                    m_dynamicModels[name] = std::make_unique<DynamicModel>(m_resourceHandler->GetResource<Model3D>(object["resource"]));
                } catch (BadResource& error) {
                    error.printError();
                } catch (std::domain_error& error) {
                    std::cout << error.what() << '\n';
                }
            }
        }
    } catch(std::exception& error) {
        Log::error("Parsing of " + filename + " failed: " + std::string(error.what()), Log::OUT_LOG_CONS);
    }
}

void GLCore::closeScene() {
    m_scene.reset(nullptr);
    m_renderChain->clear();
    lua_close(m_luaState);
}
