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

class Scene {
public:
    Scene(Display* display, lua_State* luaState, std::string scenename);
    ~Scene();

    void Start();
    void GameLoop();
    void BindFunctionToKey(int key, luabridge::LuaRef function, bool repeat);
    void AddEntity(const std::string& name, Entity* ent);

    Entity* GetEntity(const std::string& name) const;
    Camera* GetCamera() const;

private:
    std::unordered_map<std::string, std::unique_ptr<Entity>> m_entities;
    std::unordered_map<InputKey, std::unique_ptr<luabridge::LuaRef>> m_luaKeyBinds;
    std::string m_scenename;

    std::unique_ptr<luabridge::LuaRef> m_startFunc;
    std::unique_ptr<luabridge::LuaRef> m_renderFunc;

    lua_State* m_luaState;
    Display* m_display;
};

#endif // _SCENE_H
