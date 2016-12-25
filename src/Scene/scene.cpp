#include "scene.hpp"

#include <iostream>

#include <Core/camera.hpp>
#include <Utilities/exceptions.hpp>
#include <Utilities/log.hpp>

using namespace luabridge;

Scene::Scene(Display* display, lua_State* luaState, ResourceHandler* resourceHandler, std::string scenename)
        :m_display(display), m_luaState(luaState), m_resourceHandler(resourceHandler) {

    std::string script =  "Resources/Scenes/" + scenename + "/script.lua";

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
                .addFunction("MoveCamera", &Camera::MoveCamera)
                .addFunction("SetCamera", &Camera::SetPosition)
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

    luaL_dofile(m_luaState, script.c_str());

    m_startFunc = std::make_unique<LuaRef>(getGlobal(m_luaState, "Start"));
    if(!m_startFunc->isFunction()) {
        throw GenericException("Start function wasn't found");
    }

    m_renderFunc = std::make_unique<LuaRef>(getGlobal(m_luaState, "GameLoop"));
    if(!m_renderFunc->isFunction()) {
        throw GenericException("Render function wasn't found");
    }
}

Scene::~Scene() {

}

void Scene::start() {
    Entity* ent = new Entity;
    ent->setName("George");

    addEntity("George", ent);

    (*m_startFunc)();
}

void Scene::gameLoop() {
    (*m_renderFunc)();
}

void Scene::bindFunctionToKey(int ikey, LuaRef function, bool repeat) {
    if(!function.isFunction()) {
        throw GenericException("function wasn't found");
    }

    InputKey key = (InputKey)ikey;
    m_luaKeyBinds[key] = std::make_unique<LuaRef>(function);
    if(repeat) {
        m_display->GetInputController()->RegisterWhileKeyPressed(key, [this](InputKey key) {
            (*m_luaKeyBinds[key])();
        });
    } else {
        m_display->GetInputController()->RegisterOnKeyPressed(key, [this](InputKey key) {
            (*m_luaKeyBinds[key])();
        });
    }
}

Entity* Scene::spawn(const std::string& name, const std::string& resource, glm::vec3 location) {
    // TODO Spawn an entity with the given resource and name at location
    auto res = m_resourceHandler->GetResource<Model3D>(resource);
    if(res == nullptr) {
        throw BadResource("resource isn't a model or doesn't exist", resource);
    }

    if(m_entities.find(name) != m_entities.end()) {
        return m_entities[name].get();
    }

    // TODO Find location to store unqiue pointers of dynamic models that are spawned in real time
    auto ent = new Entity();
    m_entities[name] = std::unique_ptr<Entity>(ent);
    return ent;
}

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

Entity* Scene::getEntity(const std::string& name) const {
    if(m_entities.find(name) != m_entities.end()) {
        return (*m_entities.find(name)).second.get();
    }
    return nullptr;
}

Camera* Scene::getCamera() const {
    return m_display->GetCamera();
}
