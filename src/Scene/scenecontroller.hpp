#ifndef _SCENECONTROLLER_H
#define _SCENECONTROLLER_H

#include <string>
#include <unordered_map>
#include <memory>

#include <Core/display.hpp>
#include <Scene/scene.hpp>
#include <Resources/resourceloader.hpp>
#include <Render/renderobject.hpp>
#include <Render/renderchain.hpp>
#include <Entity/entity.hpp>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

class SceneController {
public:
    SceneController(std::string scenename, Display* display, RenderChain* renderchain);
    ~SceneController();

    void Start();
    void Render();

    Scene* GetScene();

private:
    void InitScene();
    void InitLuaScripts();

private:
    RenderChain* m_renderChain;
    lua_State* m_luaState;
    Display* m_display;

    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<luabridge::LuaRef> m_startFunc;
    std::unique_ptr<luabridge::LuaRef> m_renderFunc;
    std::string m_scenename;
    std::vector<std::shared_ptr<IRenderObject>> m_staticModels;
    std::unordered_map<std::string, std::shared_ptr<IRenderObject>> m_dynamicModels;
};

#endif // _SCENECONTROLLER_H
