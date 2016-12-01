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
#include <Resources/resourceloader.hpp>

using namespace luabridge;
using namespace ResourceLoader;
using json = nlohmann::json;

GLCore::GLCore(int width, int height, std::string scene)
        : m_scenename(scene), m_scene(std::make_unique<Scene>()) {

    m_display = std::make_unique<Display>(width, height);
    m_renderChain = std::make_unique<RenderChain>();

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

    InitScene();
    InitLuaScripts();
    InitControls();

    for(const auto& object : m_staticModels) {
        m_renderChain->AttachRenderObject(object);
    }

    for(const auto& object : m_dynamicModels) {
        m_renderChain->AttachRenderObject(object.second);
    }

    Entity* ent = new Entity;
    ent->SetName("George");

    m_scene->AddEntity("George", ent);

    (*m_startFunc)();
}

GLCore::~GLCore() {
    lua_close(m_luaState);
}

void GLCore::DisplayFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_renderChain->RenderObjectChain(m_display.get());
    (*m_renderFunc)();
    m_display->GetInputController()->CallKeyLambdas();
}

void GLCore::InputFunc(int key, bool state) {
    m_display->GetInputController()->UpdateKey(key, state);
}

void GLCore::MouseFunc(double xpos, double ypos) {
    m_display->GetInputController()->UpdateMousePosition(xpos, ypos);
}

void GLCore::InitScene() {
    std::string filename = "Resources/Scenes/" + m_scenename + "/scene.json";
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

        if(scene.find("staticObjects") != scene.end()) {
            std::vector<json> objects = scene["staticObjects"];
            for(json object : objects) {
                try {
                    std::string type = object["type"];

                    std::shared_ptr<IRenderObject> rObject;
                    if(type == "line") {
                        rObject = LoadLineJSON(object);
                    } else if(type == "staticmodel") {
                        rObject = LoadModelJSON(object);
                    }
                    if(rObject != nullptr) {
                        m_staticModels.push_back(rObject);
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
                    m_dynamicModels[name] = std::make_unique<DynamicModel>(LoadModel3D(filename));
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

void GLCore::InitLuaScripts() {
    std::string script =  "Resources/Scenes/" + m_scenename + "/script.lua";

    m_luaState = luaL_newstate();
    luaL_openlibs(m_luaState);

    getGlobalNamespace(m_luaState)
        .beginClass<glm::vec2>("vec2")
            .addConstructor<void(*)(float, float)>()
            .addData("x", &glm::vec2::x)
            .addData("y", &glm::vec2::y)
        .endClass()
        .beginClass<glm::vec3>("vec3")
            .addConstructor<void(*)(float, float, float)>()
            .addData("x", &glm::vec3::x)
            .addData("y", &glm::vec3::y)
            .addData("z", &glm::vec3::z)
        .endClass()
        .beginClass<glm::vec4>("vec4")
            .addConstructor<void(*)(float, float, float, float)>()
            .addData("x", &glm::vec4::x)
            .addData("y", &glm::vec4::y)
            .addData("z", &glm::vec4::z)
            .addData("w", &glm::vec4::w)
        .endClass()
        .beginNamespace("Game")
            .beginClass<Entity>("Entity")
                .addConstructor<void(*)(void)>()
                .addProperty("visible", &Entity::IsVisible, &Entity::SetVisible)
                .addProperty("name", &Entity::GetName, &Entity::SetName)
            .endClass()
            .beginClass<Scene>("Scene")
                .addConstructor<void(*)(void)>()
                .addFunction("AddEntity", &Scene::AddEntity)
                .addFunction("GetEntity", &Scene::GetEntity)
            .endClass()
        .endNamespace();

    push(m_luaState, m_scene.get());
    lua_setglobal(m_luaState, "Level");

    luaL_dofile(m_luaState, script.c_str());

    m_startFunc = std::make_unique<LuaRef>(getGlobal(m_luaState, "Start"));
    if(!m_startFunc->isFunction()) {
        throw generic_exception("Start function wasn't found");
    }

    m_renderFunc = std::make_unique<LuaRef>(getGlobal(m_luaState, "GameLoop"));
    if(!m_renderFunc->isFunction()) {
        throw generic_exception("Render function wasn't found");
    }
}

void GLCore::InitControls() {
    auto inputController = m_display->GetInputController();
    inputController->RegisterWhileKeyPressed(InputKey::A, [this]() {
        m_display->GetCamera()->MoveCamera(glm::vec3(-0.1f, 0.0f, 0.0f));
    });
    inputController->RegisterWhileKeyPressed(InputKey::S, [this]() {
        m_display->GetCamera()->MoveCamera(glm::vec3(0.0f, 0.0f, 0.1f));
    });
    inputController->RegisterWhileKeyPressed(InputKey::D, [this]() {
        m_display->GetCamera()->MoveCamera(glm::vec3(0.1f, 0.0f, 0.0f));
    });
    inputController->RegisterWhileKeyPressed(InputKey::W, [this]() {
        m_display->GetCamera()->MoveCamera(glm::vec3(0.0f, 0.0f, -0.1f));
    });
    inputController->RegisterWhileKeyPressed(InputKey::Q, [this]() {
        m_display->GetCamera()->MoveCamera(glm::vec3(0.0f, -0.1f, 0.0f));
    });
    inputController->RegisterWhileKeyPressed(InputKey::E, [this]() {
        m_display->GetCamera()->MoveCamera(glm::vec3(0.0f, 0.1f, 0.0f));
    });
}
