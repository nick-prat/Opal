#include <GL/gl.h>
#include <iostream>
#include <string>

#include "display.h"

int main(int argc, char **argv)
{
	Display* display = new Display();
	display->InitDisplay(500, 500, "OpenGL Game");
	
	RenderChain* renderChain = new RenderChain();
	
	while(!display->IsClosed())
    {
        glClearColor(0.5f, 0.25f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        display->Update();
    }
	
	display->Destroy();
	return 0;
}
