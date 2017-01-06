#include "scene.hpp"

#include <iostream>

#include <Core/camera.hpp>
#include <Models/dynamicmodel.hpp>
#include <Utilities/exceptions.hpp>
#include <Utilities/log.hpp>
#include <Render/renderobject.hpp>
#include <Render/renderchain.hpp>
#include <Resources/resourcehandler.hpp>

using namespace luabridge;
using json = nlohmann::json;

// TODO Implement scene closing
// TODO Implement some sort of multithreading in lua (coroutines?)

// NOTE How slow is calling lua functions?
// NOTE What should lua be capable of doing?

Scene::Scene(const Display* const display, lua_State* luaState, std::string scenename)
        : m_display(display), m_luaState(luaState) {

    m_renderChain = std::make_unique<RenderChain>();
    m_resourceHandler = std::make_unique<ResourceHandler>();

    std::string script =  "Resources/Scenes/" + scenename + "/script.lua";
    std::string filename = "Resources/Scenes/" + scenename + "/scene.json";

    buildLuaNamespace();

    luaL_dofile(m_luaState, script.c_str());

    registerLuaFunctions();

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

        // TODO Implement texture loading
        // NOTE What other types of resources might i want to load on scene start
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

                    // NOTE Are there other types of render obects i might want to load?
                    if(type == "line") {
                        rObject = m_resourceHandler->LoadLineJSON(object);
                    } else if(type == "staticmodel") {
                        rObject = m_resourceHandler->GenerateModel(object, m_resourceHandler->GetResource<Model3D>(object["resource"]));
                    } else if(type == "rawstaticmodel") {
                        rObject = m_resourceHandler->GenerateModel(object);
                    }

                    if(rObject != nullptr) {
                        m_renderObjects.push_back(std::unique_ptr<IRenderObject>(rObject));
                    }
                } catch (BadResource& error) {
                    error.printError();
                } catch (std::domain_error& error) {
                    Log::getErrorLog() << error.what() << '\n';
                }
            }
        }

        // TODO Implement actual dynamic model loading
        // NOTE What is "actual dynamic loading"?
        if(scene.find("dynamicObjects") != scene.end()) {
            std::vector<json> objects = scene["dynamicObjects"];
            for(json object : objects) {
                try {
                    auto name = object["name"];
                    //m_dynamicModels[name] = std::make_unique<DynamicModel>(m_resourceHandler->GetResource<Model3D>(object["resource"]));
                } catch (BadResource& error) {
                    error.printError();
                } catch (std::domain_error& error) {
                    Log::getErrorLog() << error.what() << '\n';
                }
            }
        }
    } catch(std::exception& error) {
        Log::error("Parsing of " + filename + " failed: " + std::string(error.what()), Log::OUT_LOG_CONS);
    }

    for(const auto& obj : m_renderObjects) {
        m_renderChain->attach(obj.get());
    }
}

// NOTE Make sure scene is 100% closed when *this* is deleted
Scene::~Scene() {
}

// TODO Implement entity transformation functions
// NOTE What other functions are necessary to expose to lua?
void Scene::buildLuaNamespace() {
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
            .beginClass<Camera>("Camera")
                .addFunction("MoveCamera", &Camera::moveCamera)
                .addFunction("SetCamera", &Camera::setPosition)
            .endClass()
            .beginClass<Entity>("Entity")
                .addConstructor<void(*)(void)>()
                .addProperty("visible", &Entity::isVisible, &Entity::setVisible)
                .addProperty("name", &Entity::getName, &Entity::setName)
            .endClass()
            .beginClass<Scene>("Scene")
                .addFunction("BindFunctionToKey", &Scene::bindFunctionToKey)
                .addFunction("GetCamera", &Scene::getCamera)
                .addFunction("AddEntity", &Scene::addEntity)
                .addFunction("GetEntity", &Scene::getEntity)
                .addFunction("Spawn", &Scene::spawn)
            .endClass()
        .endNamespace();

    push(m_luaState, this);
    lua_setglobal(m_luaState, "Level");
}

// NOTE Are there other necessary function the engine might want to call?
void Scene::registerLuaFunctions() {
    m_startFunc = std::make_unique<LuaRef>(getGlobal(m_luaState, "Start"));
    if(!m_startFunc->isFunction()) {
        throw GenericException("Start function wasn't found");
    }

    m_renderFunc = std::make_unique<LuaRef>(getGlobal(m_luaState, "GameLoop"));
    if(!m_renderFunc->isFunction()) {
        throw GenericException("Render function wasn't found");
    }
}

// Is this the best way to do it?
void Scene::start() {
    (*m_startFunc)();
}

// NOTE Do I want to call the render func or perform a render first?
void Scene::gameLoop() {
    m_renderChain->render(m_display);
    (*m_renderFunc)();
}

// TODO Find some way to expose what keys are pressed to lua
void Scene::bindFunctionToKey(int ikey, LuaRef function, bool repeat) {
    if(!function.isFunction()) {
        throw GenericException("function wasn't found");
    }

    InputKey key = (InputKey)ikey;
    m_luaKeyBinds[key] = std::make_unique<LuaRef>(function);
    if(repeat) {
        m_display->getInputController()->registerWhileKeyPressed(key, [this](InputKey key) {
            (*m_luaKeyBinds[key])();
        });
    } else {
        m_display->getInputController()->registerOnKeyPressed(key, [this](InputKey key) {
            (*m_luaKeyBinds[key])();
        });
    }
}

// NOTE Do i want to be able to easily destroy an entity?
Entity* Scene::spawn(const std::string& name, const std::string& resource, glm::vec3 location) {
    auto res = m_resourceHandler->GetResource<Model3D>(resource);
    if(res == nullptr) {
        throw BadResource("resource isn't a model or doesn't exist", resource);
    }

    if(m_entities.find(name) != m_entities.end()) {
        Log::getErrorLog() << "Attempting to spawn entity with name that has already been registered\n";
        return m_entities[name].get();
    }

    auto dyn = new DynamicModel(res);
    dyn->translate(location);
    m_renderChain->attach(dyn);
    m_renderObjects.push_back(std::unique_ptr<IRenderObject>(dyn));

    auto ent = new Entity();
    ent->bindModel(dyn);
    m_entities[name] = std::unique_ptr<Entity>(ent);

    return ent;
}

// NOTE What is the purpose of this?
void Scene::addEntity(const std::string& name, Entity* const ent) {
    if(m_entities.find(name) != m_entities.end()) {
        Log::error(name + " entity attempted to be added a second time, skipped");
    }

    if(ent != nullptr) {
        m_entities[name] = std::unique_ptr<Entity>(ent);
    } else {
        Log::error(name + " entity was null, skipped");
    }
}

// TODO Expand on the capabilites of an entity, ie. how does the user interact with them?
Entity* Scene::getEntity(const std::string& name) const {
    if(m_entities.find(name) != m_entities.end()) {
        return (*m_entities.find(name)).second.get();
    }
    return nullptr;
}

Camera* Scene::getCamera() const {
    return m_display->getCamera();
}
