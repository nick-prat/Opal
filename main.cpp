#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <string>

#include "display.h"
#include "renderobject.h"
#include "renderchain.h"
#include "sprite.h"
#include "utilities.hpp"

void dosomething()
{
	std::cout << "shit" << std::endl;
}

class shittyObject : public RenderObject
{
public:
    shittyObject()
    {
        verts = new Vector3f[3];
		verts[0] = Vector3f(-1.0f, -1.0f, 0.0f);
		verts[1] = Vector3f(1.0f, -1.0f, 0.0f);
		verts[2] = Vector3f(0.0f, 1.0f, 0.0f);
		
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * 3, verts, GL_STATIC_DRAW);
    }
    
	void Render()
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);
	}
	
private:
    GLuint VBO;
    Vector3f* verts;
};

int main(int argc, char **argv)
{
	Display* display = new Display();
	display->InitDisplay(1280, 720, "OpenGL Game");
	
	RenderChain* renderChain = new RenderChain();
	renderChain->InitRenderChain(10);
	
	std::cout << "Information: " << std::endl;
	std::cout << "\tGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "\tDisplay Address: " << display << std::endl;
	std::cout << "\tRender C`hain Address: " << renderChain << std::endl;
	
	shittyObject* obj = new shittyObject();
	
	while(!display->IsClosed())
    {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		
		renderChain->AttachRenderObject(obj);
		renderChain->RenderObjectChain();
	
        display->Update();
    }
	
	renderChain->Destroy();
	SafeDelete(renderChain);
	return 0;
}
