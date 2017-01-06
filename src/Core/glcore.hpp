#ifndef _GLCORE_H
#define _GLCORE_H

#include <memory>

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
    GLCore(int width, int height, std::string scene);
    ~GLCore();

    void displayFunc();
    void inputFunc(int key, bool state);
    void mouseFunc(double xpos, double ypos);

private:
    void initScene(std::string scene);
    void closeScene();

private:
    std::string m_scenename;

    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<Display> m_display;

    lua_State* m_luaState;
};

#endif // _GLCORE_H
