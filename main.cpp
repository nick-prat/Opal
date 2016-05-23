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

class ShittyObject : public RenderObject
{
public:
    ShittyObject()
    {
        m_VBO = 0;
		m_verts = nullptr;
    }
	
	~ShittyObject()
	{}
	
	bool InitObject()
	{
		m_verts = new Vector3f[3];
		
		m_verts[0] = Vector3f(-1.0f, -1.0f, 0.0f);
		m_verts[1] = Vector3f(1.0f, -1.0f, 0.0f);
		m_verts[2] = Vector3f(0.0f, 1.0f, 0.0f);
		
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * 3, m_verts, GL_STATIC_DRAW);
		
		return true;
	}
	
	void Destroy()
	{
		glDeleteBuffers(1, &m_VBO);
	}
    
	void Render()
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);
	}
	
private:
    GLuint m_VBO;
    Vector3f* m_verts;
};

int main(int argc, char **argv)
{
	Display* display = new Display();
	if(!display->InitDisplay(1280, 720, "OpenGL Game"))
	{
		std::cout << "Couldn't init display" << std::endl;
		return 1;
	}
	
	RenderChain* renderChain = new RenderChain();
	if(!renderChain->InitRenderChain(10))
	{
		std::cout << "Couldn't init render chain" << std::endl;
		return 1;
	}
	
	ShittyObject* obj = new ShittyObject();
	if(!obj->InitObject())
	{
		std::cout << "Couldn't init shitty object" << std::endl;
		return 1;
	}
	
	std::cout << "Information: " << std::endl;
	std::cout << "\tGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "\tDisplay Address: " << display << std::endl;
	std::cout << "\tRender Chain Address: " << renderChain << std::endl;
	
	while(!display->IsClosed())
    {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		
		renderChain->AttachRenderObject(obj);
		renderChain->RenderObjectChain();
	
        display->Update();
    }
	
	obj->Destroy();
	SafeDelete(obj);
	renderChain->Destroy();
	SafeDelete(renderChain);
	display->Destroy();
	SafeDelete(display);
	return 0;
}
