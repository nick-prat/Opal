#ifndef _SCENE_H
#define _SCENE_H

#include <unordered_map>
#include <string>
#include <vector>

#include <Utilities/lua.hpp>
#include <Core/display.hpp>
#include <Core/inputcontroller.hpp>
#include <ECS/ecs.hpp>
#include <ECS/Systems/rendersystem.hpp>
#include <ECS/Systems/movementsystem.hpp>
#include <Render/light.hpp>
#include <Resources/resourcehandler.hpp>

class IRenderObject;

class Scene {
public:
    using entity_manager_t = EntityManager<CRender, CLocation>;
    using render_system_t = RenderSystem<entity_manager_t>;
    using movement_system_t = MovementSystem<entity_manager_t>;
    using entity_t = Entity<entity_manager_t>;

    Scene(const Display& display, const std::string& scenename);
    Scene(const Scene&) = delete;
    Scene(Scene&& scene);
    ~Scene();

    Scene& operator=(const Scene&) = delete;
    Scene& operator=(Scene&& scene) = delete;

    void start();
    void gameLoop();

    int getEntityCount() const;

    inline ResourceHandler& getResourceHandler() {return m_resourceHandler;};

    // Lua proxy functions
    void setAmbientIntensity(float intensity);
    void setAmbientColor(const glm::vec3& color);
    void bindFunctionToKey(int key, luabridge::LuaRef function, bool repeat);
    Camera* getCamera() const;
    unsigned int createEntity();
    entity_t* getEntity(const unsigned int id) const;

private:
    void closeLua();
    void buildLuaNamespace();
    void registerLuaFunctions();
    void registerSystems();

private:
    std::vector<std::unique_ptr<IRenderObject>> m_renderObjects;
    entity_manager_t m_entityManager;
    WorldLight m_worldLight;
    ResourceHandler m_resourceHandler;
    std::vector<render_system_t> m_renderSystems;
    movement_system_t m_movementSystem;

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
