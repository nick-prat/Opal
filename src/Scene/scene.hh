#ifndef _SCENE_H
#define _SCENE_H

#include <unordered_map>
#include <string>
#include <vector>

#include <Utilities/lua.hh>
#include <Core/display.hh>
#include <Core/inputcontroller.hh>
#include <Emerald/emerald.hh>
#include <Systems/rendersystem.hh>
#include <Systems/movementsystem.hh>
#include <Render/light.hh>
#include <Resources/resourcehandler.hh>

class Scene {
public:
    Scene(const Display& display, const std::string& scenename);
    Scene(const Scene&) = delete;
    Scene(Scene&& scene);
    ~Scene();

    Scene& operator=(const Scene&) = delete;
    Scene& operator=(Scene&& scene) = delete;

    void start();
    void gameLoop();

    inline ResourceHandler& getResourceHandler() {return m_resourceHandler;};

    // Lua proxy functions
    void setAmbientIntensity(float intensity);
    void setAmbientColor(const glm::vec3& color);
    void bindFunctionToKey(int key, luabridge::LuaRef function, bool repeat);
    Camera* getCamera() const;
    std::size_t getEntityCount() const;

private:
    void closeLua();
    void buildLuaNamespace();
    void registerLuaFunctions();
    void registerSystems();

private:
    Emerald::EntityManager m_entityManager;
    WorldLight m_worldLight;
    ResourceHandler m_resourceHandler;

    const std::string m_scenename;

    // Lua related members
    LuaState m_luaState;
    std::unordered_map<InputKey, std::unique_ptr<luabridge::LuaRef>> m_luaKeyBinds;
    std::unique_ptr<luabridge::LuaRef> m_startFunc;
    std::unique_ptr<luabridge::LuaRef> m_renderFunc;

    bool m_luaEnabled;
    const Display& m_display;
};

#endif // _SCENE_H
