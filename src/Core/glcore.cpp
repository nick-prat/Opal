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
#include <Resources/resourceloader.hpp>

using namespace ResourceLoader;
using json = nlohmann::json;

GLCore::GLCore(int width, int height, std::string scene) {

    m_display = std::make_unique<Display>(width, height);
    m_renderChain = std::make_unique<RenderChain>();
    m_resourceHandler = std::make_unique<ResourceHandler>();

    // Log information about current context
    std::cout << "\nInformation: \n";
    std::cout << "\tGL Version: " << glGetString(GL_VERSION) << '\n';
    std::cout << "\tDisplay Address: " << m_display.get() << '\n';
    std::cout << "\tRender Chain Address: " << m_renderChain.get() << "\n\n";

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.1f, 0.0f, 0.0f);

    Log::info("GL Context created", Log::OUT_LOG);

    InitScene(scene);

    for(const auto& object : m_staticModels) {
        m_renderChain->Attach(object.get());
    }

    for(const auto& object : m_dynamicModels) {
        m_renderChain->Attach(object.second.get());
    }

    m_scene->Start();
}

GLCore::~GLCore() {
    CloseScene();
}

void GLCore::DisplayFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_renderChain->Render(m_display.get());
    m_scene->GameLoop();
    m_display->GetInputController()->CallKeyLambdas();
}

void GLCore::InputFunc(int key, bool state) {
    m_display->GetInputController()->UpdateKey(key, state);
}

void GLCore::MouseFunc(double xpos, double ypos) {
    m_display->GetInputController()->UpdateMousePosition(xpos, ypos);
}

void GLCore::InitScene(std::string scene) {

    m_luaState = luaL_newstate();
    luaL_openlibs(m_luaState);
    m_scene = std::make_unique<Scene>(m_display.get(), m_luaState, scene);

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
        throw generic_exception(filename + " doesn't exist");
    }

    try {
        json scene = json::parse(contents);

        if(scene.find("resources") != scene.end()) {
            std::vector<json> resources = scene["resources"];
            for(json resource : resources) {
                std::string type = resource["type"];
                std::string name = resource["name"];
                std::string filename = resource["filename"];

                if(type == "model3d") {
                    m_resourceHandler->AddResource(name, LoadModel3D(filename));
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
                        rObject = LoadLineJSON(object);
                    } else if(type == "staticmodel") {
                        rObject = new StaticModel(m_resourceHandler->GetResource<Model3D>(object["filename"]));
                    }

                    if(rObject != nullptr) {
                        m_staticModels.push_back(std::unique_ptr<IRenderObject>(rObject));
                    }
                } catch (bad_resource& error) {
                    error.PrintError();
                } catch (std::domain_error& error) {
                    std::cout << error.what() << '\n';
                }
            }
        }

        if(scene.find("dynamicObjects") != scene.end()) {
            std::vector<json> objects = scene["dynamicObjects"];
            for(json object : objects) {
                try {
                    std::string name = object["name"];
                    std::string filename = object["filename"];
                    m_dynamicModels[name] = std::make_unique<DynamicModel>(m_resourceHandler->GetResource<Model3D>(filename));
                } catch (bad_resource& error) {
                    error.PrintError();
                } catch (std::domain_error& error) {
                    std::cout << error.what() << '\n';
                }
            }
        }
    } catch(std::exception& error) {
        Log::error("Parsing of " + filename + " failed: " + std::string(error.what()), Log::OUT_LOG_CONS);
    }
}

void GLCore::CloseScene() {
    m_scene.reset(nullptr);
    m_renderChain->Clear();
    lua_close(m_luaState);
}
