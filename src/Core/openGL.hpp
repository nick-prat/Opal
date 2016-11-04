#ifndef _OPENGL_OPENGL_H
#define _OPENGL_OPENGL_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Display/display.hpp>
#include <Render/renderchain.hpp>
#include <Models/staticmodel.hpp>
#include <Models/line.hpp>

#include "glapi.hpp"

class OpenGL {
public:
    void DisplayFunc();
    void KeyboardFunc(unsigned char key, bool state, int x, int y);

    bool LoadScene(std::string name);

    static bool CreateInstance(int width, int height);
    static void DeleteInstance();
    static OpenGL*& GetInstance();

private:
    OpenGL(int width, int height);
    ~OpenGL();

private:
    static OpenGL* m_openGL;
    std::vector<std::shared_ptr<IRenderObject>> m_renderObjects;
    std::shared_ptr<GlutDisplay> m_display;
    std::vector<std::shared_ptr<Line>> m_lines;
    std::vector<std::shared_ptr<StaticModel>> m_staticModels;
};

#endif //OPENGL_OPENGL_H
