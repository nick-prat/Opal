#ifndef _GLCORE_H
#define _GLCORE_H

#include <Opal/Core/display.hh>
#include <Opal/Scene/scene.hh>

#include <memory>
#include <glm/glm.hpp>

// TODO Be able to switch between what window is active

namespace Opal {

    class GLCore {
    public:
        GLCore(int width, int height, std::string scene);
        GLCore(const GLCore&) = delete;
        GLCore(GLCore&& glCore);

        GLCore& operator=(const GLCore&) = delete;
        GLCore& operator=(GLCore&& glCore);

        void start();

        Display& getDisplay();
        const Display& getDisplay() const;
        Scene& getCurrentScene();
        const Scene& getCurrentScene() const;


        template<typename scene_t, typename... args_t>
        void loadScene(const std::string& scenename, args_t&&... args) {
            static_assert(std::is_base_of<Scene, scene_t>::value, "Scene type doesn't inherit base Scene");
            m_scene = std::make_unique<scene_t>(m_display, scenename, std::forward<args_t>(args)...);
        }

    private:
        Display m_display;
        std::unique_ptr<Scene> m_scene;
    };

}

#endif // _GLCORE_H
