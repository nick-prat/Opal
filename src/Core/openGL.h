#ifndef _OPENGL_OPENGL_H
#define _OPENGL_OPENGL_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Model/renderchain.h>
#include <Display/display.h>
#include <Model/Models/staticmodel.h>
#include <Model/testObject.h>

#include "glapi.h"

class OpenGL
{
public:
    void DisplayFunc();
    void KeyboardFunc(unsigned char key, bool state, int x, int y);

    static bool CreateInstance(int width, int height, std::string title);
    static void DeleteInstance();
    static OpenGL*& GetInstance();

private:
    OpenGL(int width, int height, std::string title);
    ~OpenGL();

private:
    static OpenGL* m_openGL;

    long m_lowestTime;
    std::shared_ptr<GlutDisplay> m_display;
    std::shared_ptr<StaticModel> m_staticModel;
    std::shared_ptr<TestObject> m_obj;
    std::shared_ptr<TestObject> m_obj2;
};

#endif //OPENGL_OPENGL_H
