#ifndef _SCENE_H
#define _SCENE_H

#include <Opal/Util/lua.hh>
#include <Opal/Core/display.hh>
#include <Opal/Systems/rendersystem.hh>
#include <Opal/Systems/movementsystem.hh>
#include <Opal/Resources/assetstore.hh>
#include <Opal/Render/light.hh>

#include <Emerald/emerald.hh>
#include <unordered_map>
#include <string>
#include <vector>

namespace Opal {

    class Scene {
    public:
        Scene(Display& display, const std::string& scenename);
        Scene(const Scene&) = delete;
        Scene(Scene&& scene);
        virtual ~Scene();

        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&& scene) = delete;

        void start();
        void gameLoop();

        inline AssetStore& getAssetStore() {return m_assetStore;};

        // Lua proxy functions
        void setAmbientIntensity(float intensity);
        void setAmbientColor(const glm::vec3& color);
        void bindFunctionToKey(int key, luabridge::LuaRef function, bool repeat);
        inline Camera& getCamera();
        std::size_t getEntityCount() const;

    protected:
        virtual void registerSystems();

    private:
        void closeLua();
        void buildLuaNamespace();
        void registerLuaFunctions();

    protected:
        Emerald::EntityManager m_entityManager;
        WorldLight m_worldLight;
        AssetStore m_assetStore;
        Display& m_display;
        const std::string m_scenename;

    private:
        LuaState m_luaState;
        std::unordered_map<InputKey, std::unique_ptr<luabridge::LuaRef>> m_luaKeyBinds;
        std::unique_ptr<luabridge::LuaRef> m_startFunc;
        std::unique_ptr<luabridge::LuaRef> m_renderFunc;
        bool m_luaEnabled;
    };

}

#endif // _SCENE_H
