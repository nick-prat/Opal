#ifndef _GLCORE_H
#define _GLCORE_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Core/display.hpp>
#include <Render/renderchain.hpp>
#include <Models/staticmodel.hpp>
#include <Models/line.hpp>
#include <Scene/scene.hpp>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

class GLCore {
public:
    GLCore(int width, int height, std::string scene);
    ~GLCore();

    void DisplayFunc();
    void InputFunc(int key, bool state);
    void MouseFunc(double xpos, double ypos);

private:
    void InitScene();
    void InitControls();

private:
    std::string m_scenename;
    std::unordered_map<std::string, std::shared_ptr<IRenderObject>> m_dynamicModels;
    std::vector<std::shared_ptr<IRenderObject>> m_renderObjects;
    std::vector<std::shared_ptr<Line>> m_lines;
    std::vector<std::shared_ptr<IRenderObject>> m_staticModels;

    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<RenderChain> m_renderChain;
    std::unique_ptr<Display> m_display;
};

#endif // _GLCORE_H
