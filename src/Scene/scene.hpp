#ifndef _SCENE_H
#define _SCENE_H

#include <unordered_map>
#include <string>
#include <vector>

#include <Utilities/lua.hpp>
#include <Core/display.hpp>
#include <Core/inputcontroller.hpp>
#include <Entity/components.hpp>
#include <Entity/entity.hpp>
#include <Render/renderchain.hpp>
#include <Resources/resourcehandler.hpp>

class IRenderObject;

class Scene {
    using entity_manager_t = EntityManager<CRender, CLocation>;
    using entity_t = entity_manager_t::Entity;
public:
    Scene();
    Scene(const Display* const display, std::string scenename);
    Scene(const Scene&) = delete;
    Scene(Scene&& scene);
    ~Scene();

    Scene& operator=(const Scene&) = delete;
    Scene& operator=(Scene&& scene);

    void start();
    void gameLoop();

    int getEntityCount() const;

    inline RenderChain& getRenderChain() {return m_renderChain;};
    inline ResourceHandler& getResourceHandler() {return m_resourceHandler;};

    // Lua proxy functions
    void setAmbientIntensity(float intensity);
    void setAmbientColor(const glm::vec3& color);
    void bindFunctionToKey(int key, luabridge::LuaRef function, bool repeat);
    Camera* getCamera() const {return m_display->getCamera();};
    unsigned int createEntity();
    entity_t* getEntity(const unsigned int id) const;

private:
    void closeLua();
    void buildLuaNamespace();
    void registerLuaFunctions();

private:
    std::vector<std::unique_ptr<IRenderObject>> m_renderObjects;
    entity_manager_t m_entityManager;
    RenderChain m_renderChain;
    ResourceHandler m_resourceHandler;
    std::string m_scenename;

    // Lua related members
    LuaState m_luaState;
    std::unordered_map<InputKey, std::unique_ptr<luabridge::LuaRef>> m_luaKeyBinds;
    std::unique_ptr<luabridge::LuaRef> m_startFunc;
    std::unique_ptr<luabridge::LuaRef> m_renderFunc;

    bool m_luaEnabled;
    const Display* m_display;
};

#endif // _SCENE_H
