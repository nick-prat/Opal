#include <Opal/Scene/scene.hh>
#include <Opal/Core/gl.hh>
#include <Opal/Core/camera.hh>
#include <Opal/Util/exceptions.hh>
#include <Opal/Util/log.hh>

#include <iostream>
#include <fstream>

#include <LuaBridge/LuaBridge.h>
#include <LuaBridge/LuaBridge.h>

using luabridge::LuaRef;

// TODO Implement some sort of multithreading in lua (coroutines?)
// TODO Check to see if main.lua exists (a scene doesn't need to have lua support)

// NOTE How slow is calling lua functions?
// NOTE What should lua be capable of doing?

Opal::Scene::Scene(Display& display, const std::string& scenename)
: m_assetStore{scenename}
, m_display{display}
, m_scenename{scenename}
, m_luaEnabled{true} {
    m_display.setWireFrame(false);

    luaL_openlibs(m_luaState.get());

    std::string script =  "Resources/Scenes/" + scenename + "/main.lua";

    buildLuaNamespace();

    luaL_dofile(m_luaState.get(), script.c_str());

    registerLuaFunctions();

    m_worldLight.setAmbientColor({1.0f, 1.0f, 1.0f});
    m_worldLight.setAmbientIntensity(0.6f);
}

Opal::Scene::Scene(Scene&& scene)
: m_entityManager{std::move(scene.m_entityManager)}
, m_assetStore{std::move(scene.m_assetStore)}
, m_display{scene.m_display}
, m_scenename{scene.m_scenename}
, m_luaState{std::move(scene.m_luaState)}
, m_luaKeyBinds{std::move(scene.m_luaKeyBinds)}
, m_startFunc{std::move(scene.m_startFunc)}
, m_renderFunc{std::move(scene.m_renderFunc)}
, m_luaEnabled{scene.m_luaEnabled} {
    scene.m_luaEnabled = false;
}

Opal::Scene::~Scene() {}

// NOTE What other functions are necessary to expose to lua?
void Opal::Scene::buildLuaNamespace() {
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
                .addFunction("Update", &Camera::update)
            .endClass()
            .beginClass<Scene>("Scene")
                .addFunction("SetAmbientIntensity", &Opal::Scene::setAmbientIntensity)
                .addFunction("SetAmbientColor", &Opal::Scene::setAmbientColor)
                .addFunction("BindFunctionToKey", &Opal::Scene::bindFunctionToKey)
                .addFunction("GetCamera", &Opal::Scene::getCamera)
                .addFunction("GetEntityCount", &Opal::Scene::getEntityCount)
            .endClass()
        .endNamespace();

    luabridge::setGlobal(m_luaState.get(), this, "Level");
}

// NOTE Are there other necessary function the engine might want to call?
void Opal::Scene::registerLuaFunctions() {
    m_startFunc = std::make_unique<LuaRef>(luabridge::getGlobal(m_luaState.get(), "Start"));
    if(!m_startFunc->isFunction()) {
        m_startFunc.release();
        // throw GenericException("Start function wasn't found");
    }

    m_renderFunc = std::make_unique<LuaRef>(luabridge::getGlobal(m_luaState.get(), "GameLoop"));
    if(!m_renderFunc->isFunction()) {
        m_renderFunc.release();
        // throw GenericException("Render function wasn't found");
    }
}

void Opal::Scene::registerSystems() {
    m_entityManager.registerSystem<ModelRenderSystem>(m_assetStore.getShader(ModelRenderSystem::shaderName), m_display, m_worldLight);
    m_entityManager.registerSystem<MovementSystem>();
}

void Opal::Scene::start() {
    registerSystems();

    if(m_startFunc) {
        (*m_startFunc)();
    }
}

void Opal::Scene::gameLoop() {
    m_entityManager.updateSystems();
    if(m_renderFunc) {
        (*m_renderFunc)();
    }
}

// TODO Find some way to expose what keys are pressed to lua
void Opal::Scene::bindFunctionToKey(int ikey, LuaRef function, bool repeat) {
    if(!function.isFunction()) {
        throw GenericException("function wasn't found");
    }

    InputKey key = (InputKey)ikey;
    m_luaKeyBinds[key] = std::make_unique<LuaRef>(function);
    if(repeat) {
        m_display.registerWhileKeyPressed(key, [this](InputKey key) {
            m_luaKeyBinds[key]->operator()();
        });
    } else {
        m_display.registerOnKeyPressed(key, [this](InputKey key) {
            m_luaKeyBinds[key]->operator()();
        });
    }
}

void Opal::Scene::setAmbientIntensity(float intensity) {
    m_worldLight.setAmbientIntensity(intensity);
}

void Opal::Scene::setAmbientColor(const glm::vec3& color) {
    m_worldLight.setAmbientColor(color);
}

Opal::Camera& Opal::Scene::getCamera() {
    return m_display.getCamera();
}

std::size_t Opal::Scene::getEntityCount() const {
    return m_entityManager.getEntityCount();
}
