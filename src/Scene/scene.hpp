#ifndef _SCENE_H
#define _SCENE_H

#include <unordered_map>
#include <string>
#include <list>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

#include <Core/display.hpp>
#include <Core/inputcontroller.hpp>
#include <Entity/entity.hpp>

class IRenderObject;
class ResourceHandler;
class RenderChain;

class Scene {
public:
    Scene(Display* display, lua_State* luaState, std::string scenename);
    ~Scene();

    void start();
    void gameLoop();
    void bindFunctionToKey(int key, luabridge::LuaRef function, bool repeat);
    void addEntity(const std::string& name, Entity* const ent);

    Entity* spawn(const std::string& name, const std::string& resource, glm::vec3 location);
    Entity* getEntity(const std::string& name) const;
    Camera* getCamera() const;

private:
    void buildLuaNamespace();
    void registerLuaFunctions();

private:
    std::unordered_map<std::string, std::unique_ptr<Entity>> m_entities;
    std::unordered_map<InputKey, std::unique_ptr<luabridge::LuaRef>> m_luaKeyBinds;
    std::list<std::unique_ptr<IRenderObject>> m_renderObjects;
    std::string m_scenename;

    std::unique_ptr<luabridge::LuaRef> m_startFunc;
    std::unique_ptr<luabridge::LuaRef> m_renderFunc;
    std::unique_ptr<RenderChain> m_renderChain;
    std::unique_ptr<ResourceHandler> m_resourceHandler;

    Display* m_display;
    lua_State* m_luaState;
};

#endif // _SCENE_H
