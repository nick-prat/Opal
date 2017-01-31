#ifndef _GLCORE_H
#define _GLCORE_H

#include <memory>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

class ResourceHandler;
class Scene;
class Display;

class GLCore {
public:
    GLCore();
    GLCore(int width, int height, std::string scene);
    GLCore(const GLCore& rhs) = delete;
    GLCore(GLCore&& glCore);
    ~GLCore();

    GLCore& operator=(GLCore&& glCore);

    bool shouldClose() const;

    void setClearColor(const glm::vec4& color);
    void setVsync(bool enabled);

    GLFWwindow* getWindow() const;
    const Display* getDisplay() const;
    Scene* getCurrentScene() const;
    Scene* createScene(const std::string& scenename);
    void startScene(Scene* scene);

    void displayFunc();
    void inputFunc(int key, bool state);
    void mouseFunc(double xpos, double ypos);

private:
    std::function<void()> m_deleter;
    std::unique_ptr<const Display> m_display;
    Scene* m_currentScene;
    GLFWwindow* m_window;
    lua_State* m_luaState;
};

#endif // _GLCORE_H
