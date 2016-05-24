#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/constants.hpp>
#include <iostream>
#include <string>
#include <limits>

#include "display.h"
#include "renderobject.h"
#include "renderchain.h"
#include "sprite.h"
#include "utilities.hpp"
#include "camera.h"

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
	
	bool InitObject(Display* display, Camera* camera)
	{
		m_display = display;
		m_camera = camera;
		m_verts = new glm::vec3[3];
		
		m_verts[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
		m_verts[1] = glm::vec3(1.0f, -1.0f, 0.0f);
		m_verts[2] = glm::vec3(0.0f, 1.0f, 0.0f);
		
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * 3, m_verts, GL_STATIC_DRAW);
		
		m_shader = new Shader();
		std::vector<std::string> files = {"shader.vs", "shader.fs"};
		std::vector<GLenum> types = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
		if(!m_shader->InitShader(files, types))
		{
			std::cout << "Couldn't initialize shader" << std::endl;
			return 1;
		}
		
		return true;
	}
	
	void Destroy()
	{
		glDeleteBuffers(1, &m_VBO);
		m_VBO = 0;
		m_shader->Destroy();
		SafeDelete(m_shader);
	}
    
	void Render()
	{
		m_shader->UseShader();
		
		//glm::mat4 world = m_translate * m_rotate * m_scale;
		glm::mat4 model =  m_translate * m_rotate * m_scale;
		glm::mat4 mvp = m_display->GetProjectionMatrix() * m_camera->GetViewMatrix() * model;
		
		GLint worldLocation = glGetUniformLocation(m_shader->GetProgram(), "gMVP");
		if(worldLocation == -1)
		{
			std::cout << "Couldn't get uniform loaction" << std::endl;
		}
		glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(mvp));
		
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);
	}
	
private:
    GLuint m_VBO;
	glm::vec3* m_verts;
	Shader* m_shader;
};

int main(int argc, char **argv)
{
	Display* display = new Display();
	if(!display->InitDisplay(1280, 720, "OpenGL Game"))
	{
		std::cout << "Couldn't init display" << std::endl;
		return 1;
	}
	
	Camera* camera = new Camera();
	if(!camera->InitCamera())
	{
		std::cout << "Couldn't init camera" << std::endl;
		return 1;
	}
	
	RenderChain* renderChain = new RenderChain();
	if(!renderChain->InitRenderChain(10))
	{
		std::cout << "Couldn't init render chain" << std::endl;
		return 1;
	}
	
	ShittyObject* obj = new ShittyObject();
	if(!obj->InitObject(display, camera))
	{
		std::cout << "Couldn't init shitty object" << std::endl;
		return 1;
	}
	
	obj->Scale(glm::scale(glm::vec3(0.5f, 1.0f, 1.0f)));
	obj->Rotate(glm::rotate(glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)));
	obj->Translate(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	
	std::cout << "Information: " << std::endl;
	std::cout << "\tGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "\tDisplay Address: " << display << std::endl;
	std::cout << "\tRender Chain Address: " << renderChain << std::endl;
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	while(!display->IsClosed())
    {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
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
