#ifndef _GLCORE_H
#define _GLCORE_H

#include <memory>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <GLFW/glfw3.h>

class ResourceHandler;
class Scene;
class Display;

class GLCore {
public:
    GLCore(int width, int height, std::string scene);
    ~GLCore();

    bool shouldClose() const;

    GLFWwindow* getWindow() const;
    Display* getDisplay() const;

    Scene* getCurrentScene() const;
    Scene* createScene(const std::string& scenename);
    void startScene(Scene* scene);

    void displayFunc();
    void inputFunc(int key, bool state);
    void mouseFunc(double xpos, double ypos);

private:
    std::unique_ptr<const Display> m_display;

    Scene* m_currentScene;
    GLFWwindow* m_window;
    lua_State* m_luaState;
};

#endif // _GLCORE_H
