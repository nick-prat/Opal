#include "scenecontroller.hpp"

#include <iostream>
#include <fstream>

#include <Resources/resourceloader.hpp>
#include <Utilities/log.hpp>
#include <Utilities/exceptions.hpp>

using json = nlohmann::json;
using namespace luabridge;

SceneController::SceneController(std::string scenename, Display* display, RenderChain* renderChain)
        :m_renderChain(renderChain), m_luaState(nullptr), m_display(display), m_scenename(scenename) {
    InitScene();
    InitLuaScripts();

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

    for(const auto& object : m_staticModels) {
        m_renderChain->AttachRenderObject(object);
    }

    for(const auto& object : m_dynamicModels) {
        m_renderChain->AttachRenderObject(object.second);
    }

    Entity* ent = new Entity;
    ent->SetName("George");

    m_scene = std::make_unique<Scene>();
    m_scene->AddEntity("George", ent);

    Start();
}

SceneController::~SceneController() {
    lua_close(m_luaState);
}

void SceneController::Start() {
    (*m_startFunc)(m_scene.get());
}

void SceneController::Render() {
    (*m_renderFunc)();
}

void SceneController::InitScene() {
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
                        rObject = ResourceLoader::LoadLineJSON(object);
                    } else if(type == "staticmodel") {
                        rObject = ResourceLoader::LoadModelJSON(object);
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
                    std::string type = object["type"];
                    std::string name = object["name"];

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

void SceneController::InitLuaScripts() {
    std::string script =  "Resources/Scenes/" + m_scenename + "/script.lua";

    m_luaState = luaL_newstate();
    luaL_openlibs(m_luaState);

    getGlobalNamespace(m_luaState)
        .beginNamespace("Game")
            .beginClass<Entity>("Entity")
                .addConstructor<void(*)(void)>()
                .addProperty("name", &Entity::GetName, &Entity::SetName)
            .endClass()
            .beginClass<Scene>("Scene")
                .addConstructor<void(*)(void)>()
                .addFunction("GetEntity", &Scene::GetEntity)
            .endClass()
        .endNamespace();

    luaL_dofile(m_luaState, script.c_str());

    m_startFunc = std::make_unique<LuaRef>(getGlobal(m_luaState, "Start"));
    m_renderFunc = std::make_unique<LuaRef>(getGlobal(m_luaState, "GameLoop"));

    if(!m_startFunc->isFunction() || !m_renderFunc->isFunction()) {
        throw generic_exception("start and/or render function weren't found");
    }
}
