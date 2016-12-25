#ifndef _SCENE_H
#define _SCENE_H

#include <unordered_map>
#include <string>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

#include <Core/display.hpp>
#include <Entity/entity.hpp>
#include <Resources/resourcehandler.hpp>

class Scene {
public:
    Scene(Display* display, lua_State* luaState, ResourceHandler* resourceHandler, std::string scenename);
    ~Scene();

    void start();
    void gameLoop();
    void bindFunctionToKey(int key, luabridge::LuaRef function, bool repeat);
    void addEntity(const std::string& name, Entity* const ent);

    Entity* spawn(const std::string& name, const std::string& resource, glm::vec3 location);
    Entity* getEntity(const std::string& name) const;
    Camera* getCamera() const;

private:
    std::unordered_map<std::string, std::unique_ptr<Entity>> m_entities;
    std::unordered_map<InputKey, std::unique_ptr<luabridge::LuaRef>> m_luaKeyBinds;
    std::string m_scenename;

    std::unique_ptr<luabridge::LuaRef> m_startFunc;
    std::unique_ptr<luabridge::LuaRef> m_renderFunc;

    Display* m_display;
    lua_State* m_luaState;
    ResourceHandler* m_resourceHandler;
};

#endif // _SCENE_H
