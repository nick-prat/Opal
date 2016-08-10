//
// Created by nprat on 7/4/16.
//

#ifndef _OPENGL_OPENGL_H
#define _OPENGL_OPENGL_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Model/renderchain.h>
#include <Display/display.h>
#include <Model/testObject.h>

#include "glapi.h"

class OpenGL
{
public:
    OpenGL();
    ~OpenGL();

    void DisplayFunc();

    static bool CreateInstance(int width, int height, std::string title);
    static void DeleteInstance();
    static OpenGL* GetInstance();

private:
    bool InitOpenGL(int width, int height, std::string title);
    void Destroy();

private:
    static OpenGL* m_openGL;

private:
    long m_lowestTime;
    std::shared_ptr<GlutDisplay> m_display;
    std::shared_ptr<TestObject> m_obj;
    std::shared_ptr<TestObject> m_obj2;
};


#endif //OPENGL_OPENGL_H
