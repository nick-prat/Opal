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
    Scene();
    Scene(const Display* const display, std::string scenename);
    Scene(const Scene&) = delete;
    Scene(Scene&& scene);
    ~Scene();

    Scene& operator=(const Scene&) = delete;
    Scene& operator=(Scene&& scene);
    void destroy();

    void start();
    void gameLoop();
    void bindFunctionToKey(int key, luabridge::LuaRef function, bool repeat);
    void addEntity(const std::string& name, Entity* const ent);

    void setAmbientIntensity(float intensity);
    void setAmbientColor(const glm::vec3& color);
    Entity* spawn(const std::string& name, const std::string& resource, glm::vec3 location);
    Entity* getEntity(const std::string& name) const;
    int getEntityCount() const;
    Camera* getCamera() const;

private:
    void closeLua();
    void buildLuaNamespace();
    void registerLuaFunctions();

private:
    std::unordered_map<std::string, std::unique_ptr<Entity>> m_entities;
    std::list<std::unique_ptr<IRenderObject>> m_renderObjects;
    std::unique_ptr<RenderChain> m_renderChain;
    std::unique_ptr<ResourceHandler> m_resourceHandler;
    std::string m_scenename;

    // Lua pointers
    std::unordered_map<InputKey, std::unique_ptr<luabridge::LuaRef>> m_luaKeyBinds;
    std::unique_ptr<luabridge::LuaRef> m_startFunc;
    std::unique_ptr<luabridge::LuaRef> m_renderFunc;
    lua_State* m_luaState;

    bool m_luaEnabled;
    const Display* m_display;
};

#endif // _SCENE_H
