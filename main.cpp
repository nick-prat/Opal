#include <GL/gl.h>
#include <iostream>
#include <string>

#include "display.h"
#include "renderchain.h"
#include "sprite.h"
#include "utilities.hpp"

int main(int argc, char **argv)
{
	Display* display = new Display();
	display->InitDisplay(500, 500, "OpenGL Game");
	
	RenderChain* renderChain = new RenderChain();
	renderChain->InitRenderChain(10);
	
	std::cout << "Devices: " << std::endl;
	std::cout << "\tDisplay Address: " << display << std::endl;
	std::cout << "\tRender C`hain Address: " << renderChain << std::endl;
	
	while(!display->IsClosed())
    {
        glClearColor(0.5f, 0.25f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        display->Update();
    }
	
	renderChain->Destroy();
	SafeDelete(renderChain);
	return 0;
}
