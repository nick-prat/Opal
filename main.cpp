#include <GL/gl.h>
#include <iostream>
#include <string>

#include "Display/display.h"
#include "Shader/shader.h"

using namespace std;

int main()
{
    Display* display = new Display();
    if(!display->InitDisplay(800, 600, "First OpenGL Project"))
    {
        cerr << "Couldn't start display" << endl;
        return 1;
    }

    Shader* shader = new Shader();
    if(!shader->InitShader())
    {
        cerr << "Couldn't load shader" << endl;
        return 1;
    }

    cout << "Display address: " << display << endl;

    while(!display->IsClosed())
    {
        glClearColor(0.5f, 0.25f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        display->Update();
    }

    return 0;
}
