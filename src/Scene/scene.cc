#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "scene.hh"

#include <iostream>

#include <LuaBridge/LuaBridge.h>
#include <Core/camera.hh>
#include <Utilities/exceptions.hh>
#include <Utilities/log.hh>
#include <Render/renderobject.hh>
#include <Resources/resourcehandler.hh>
#include <LuaBridge/LuaBridge.h>

using luabridge::LuaRef;
using json = nlohmann::json;

// TODO Implement some sort of multithreading in lua (coroutines?)
// TODO Check to see if main.lua exists (a scene doesn't need to have lua support)

// NOTE How slow is calling lua functions?
// NOTE What should lua be capable of doing?

Scene::Scene(const Display& display, const std::string& scenename)
: m_scenename(scenename)
, m_luaEnabled(true)
, m_display(display) {
    luaL_openlibs(m_luaState.get());

    std::string script =  "Resources/Scenes/" + scenename + "/main.lua";
    std::string filename = "Resources/Scenes/" + scenename + "/scene.json";

    buildLuaNamespace();

    luaL_dofile(m_luaState.get(), script.c_str());

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
        const json scene = json::parse(contents);
        m_resourceHandler.loadResources(scene);

        if(scene.find("staticObjects") != scene.end()) {
            std::vector<json> objects = scene["staticObjects"];
            for(const auto& object : objects) {
                try {
                    std::string type = object["type"];
                    std::string name = "unknown";
                    if(auto ni = object.find("name"); ni != object.end()) {
                        name = *ni;
                    }
                    // NOTE Are there other types of render obects i might want to load?
                    if(type == "line") {
                        if(IRenderObject* robj = m_resourceHandler.generateLine(object); robj != nullptr) {
                            m_renderObjects.push_back(std::unique_ptr<IRenderObject>(robj));
                        }
                    } else if(type == "staticmodel") {
                        std::cout << "Creating staticmodel\n";
                        auto id = m_entityManager.createEntity();
                        m_entityManager.createComponent<CRender>(id, m_resourceHandler.getModel3D(object["resource"]));
                        m_entityManager.createComponent<CBody>(id);
                        auto& cbody = m_entityManager.getComponent<CBody>(id);

                        std::vector<float> loc, scl, rot;

                        if(auto ti = object.find("translation"); ti != object.end()) {
                            if((*ti).size() == 3) {
                                cbody.setLocation({(*ti)[0], (*ti)[1], (*ti)[2]});
                            } else {
                                Log::getErrorLog() << "invalid translation format for " << name << " size was " << (*ti).size() << " expected 3\n";
                            }
                        }

                        if(auto ri = object.find("rotation"); ri != object.end()) {
                            if((*ri).size() == 3) {
                                cbody.setRotation({(*ri)[0], (*ri)[1], (*ri)[2]});
                            } else {
                                Log::getErrorLog() << "invalid rotation format for " << name << " size was " << (*ri).size() << " expected 3\n";
                            }
                        }

                        if(auto si = object.find("scale"); si != object.end() && (*si).size() == 3) {
                            if((*si).size() == 3) {
                                cbody.setScale({(*si)[0], (*si)[1], (*si)[2]});
                            } else {
                                Log::getErrorLog() << "invalid scale format for " << name << " size was " << (*si).size() << " expected 3\n";
                            }
                        }
                    } else {
                        Log::getErrorLog() << "Unknown type " << type << " for " << name << " skipping...\n";
                        continue;
                    }
                } catch (BadResource& error) {
                    error.printError();
                } catch (std::domain_error& error) {
                    Log::getErrorLog() << error.what() << '\n';
                }
            }
        }
    } catch(std::exception& error) {
        Log::getErrorLog() << "Parsing of " << filename << " failed: " << error.what() << '\n';
    }

    m_worldLight.setAmbientColor({1.0f, 1.0f, 1.0f});
    m_worldLight.setAmbientIntensity(1.0f);

    registerSystems();
}

Scene::Scene(Scene&& scene)
: m_renderObjects(std::move(scene.m_renderObjects))
, m_entityManager(std::move(scene.m_entityManager))
, m_resourceHandler(std::move(scene.m_resourceHandler))
, m_scenename(scene.m_scenename)
, m_luaState(std::move(scene.m_luaState))
, m_luaKeyBinds(std::move(scene.m_luaKeyBinds))
, m_startFunc(std::move(scene.m_startFunc))
, m_renderFunc(std::move(scene.m_renderFunc))
, m_luaEnabled(scene.m_luaEnabled)
, m_display(scene.m_display) {
    scene.m_luaEnabled = false;
}

Scene::~Scene() {}

// NOTE What other functions are necessary to expose to lua?
void Scene::buildLuaNamespace() {
    luabridge::getGlobalNamespace(m_luaState.get())
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
                .addFunction("GetPosition", &Camera::getPosition)
                .addFunction("GetDirection", &Camera::getDirection)
            .endClass()
            .beginClass<Scene>("Scene")
                .addFunction("SetAmbientIntensity", &Scene::setAmbientIntensity)
                .addFunction("SetAmbientColor", &Scene::setAmbientColor)
                .addFunction("BindFunctionToKey", &Scene::bindFunctionToKey)
                .addFunction("GetCamera", &Scene::getCamera)
                // .addFunction("GetEntityCount", &Scene::getEntityCount)
            .endClass()
        .endNamespace();

    luabridge::setGlobal(m_luaState.get(), this, "Level");
}

// NOTE Are there other necessary function the engine might want to call?
void Scene::registerLuaFunctions() {
    m_startFunc = std::make_unique<LuaRef>(luabridge::getGlobal(m_luaState.get(), "Start"));
    if(!m_startFunc->isFunction()) {
        throw GenericException("Start function wasn't found");
    }

    m_renderFunc = std::make_unique<LuaRef>(luabridge::getGlobal(m_luaState.get(), "GameLoop"));
    if(!m_renderFunc->isFunction()) {
        throw GenericException("Render function wasn't found");
    }
}

void Scene::registerSystems() {
    m_entityManager.registerSystem<ModelRenderSystem>(m_resourceHandler.getShader(ModelRenderSystem::shaderName), m_display, m_worldLight);
    m_entityManager.registerSystem<MovementSystem>();
}

// Is this the best way to do it?
void Scene::start() {
    (*m_startFunc)();
}

// NOTE Do I want to call the render func or perform a render first?
void Scene::gameLoop() {
    m_entityManager.updateSystems(1.0f);
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
        m_display.getInputController()->registerWhileKeyPressed(key, [this](InputKey key) {
            m_luaKeyBinds[key]->operator()();
        });
    } else {
        m_display.getInputController()->registerOnKeyPressed(key, [this](InputKey key) {
            m_luaKeyBinds[key]->operator()();
        });
    }
}

void Scene::setAmbientIntensity(float intensity) {
    m_worldLight.setAmbientIntensity(intensity);
}

void Scene::setAmbientColor(const glm::vec3 &color) {
    m_worldLight.setAmbientColor(color);
}

Camera* Scene::getCamera() const {
    return m_display.getCamera();
}

// int Scene::getEntityCount() const {
//     return m_entityManager.getEntityCount();
// }
