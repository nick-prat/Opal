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

#include <Core/display.hh>

class Scene;

// TODO Be able to switch between what window is active

class GLCore {
public:
    GLCore();
    GLCore(int width, int height, std::string scene);
    GLCore(const GLCore&) = delete;
    GLCore(GLCore&& glCore);
    ~GLCore();

    GLCore& operator=(const GLCore&) = delete;
    GLCore& operator=(GLCore&& glCore);
    void destroy();

    static void initAPI();
    static void closeAPI();

    void start();

    void setClearColor(const glm::vec4& color);
    void setVsync(bool enabled);

    GLFWwindow* getWindow() const;
    const Display& getDisplay() const;
    Scene* getCurrentScene() const;
    void loadScene(const std::string& scenename);

private:
    void inputFunc(int key, bool state);
    void mouseFunc(double xpos, double ypos);

private:
    std::unique_ptr<Scene> m_scene;
    Display m_display;
    GLFWwindow* m_window;
};

#endif // _GLCORE_H
