#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "Utilities/utilities.hpp"
#include "Display/display.h"
#include "Model/Shader/shader.h"
#include "Model/renderchain.h"

using Utilities::SafeDelete;

class ShittyObject : public RenderObject
{
public:
    ShittyObject()
    {
        m_VBO = 0;
		m_IBO = 0;
		m_verts = nullptr;
		m_shader = nullptr;
		m_indices = nullptr;
    }

	~ShittyObject()
	{}

	bool InitObject(Display* display)
	{
		m_display = display;

		m_verts = new glm::vec3[4];
		m_verts[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
		m_verts[1] = glm::vec3(1.0f, -1.0f, 0.0f);
		m_verts[2] = glm::vec3(-1.0f, 1.0f, 0.0f);
		m_verts[3] = glm::vec3(1.0f, 1.0f, 0.0f);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 4, m_verts, GL_STATIC_DRAW);

		delete [] m_verts;

		m_indices = new uint[6];
		m_indices[0] = 0;
		m_indices[1] = 1;
		m_indices[2] = 2;
		m_indices[3] = 1;
		m_indices[4] = 3;
		m_indices[5] = 2;

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, m_indices, GL_STATIC_DRAW);

		delete [] m_indices;

		m_shader = new Shader();
		//std::vector<std::string> files = {"Shaders/shader.vs", "Shaders/shader.fs"};
		//std::vector<GLenum> types = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
		std::vector<std::string> files = {"Shaders/shader.vs", "Shaders/shader.fs", "Shaders/shader.gs"};
		std::vector<GLenum> types = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER};
		if(!m_shader->InitShader(files, types))
		{
			std::cout << "Couldn't initialize shader" << std::endl;
			return false;
		}

		return true;
	}

	void Destroy()
	{
		glDeleteBuffers(1, &m_VBO);
		m_VBO = 0;
		glDeleteBuffers(1, &m_IBO);
		m_IBO = 0;
		m_shader->Destroy();
		SafeDelete(m_shader);
	}

	void Render()
	{
		m_shader->UseShader();

		glm::mat4 mvp = m_display->GetProjectionMatrix() * m_display->GetCameraModule()->GetViewMatrix() * GetWorld();

		GLint worldLocation = glGetUniformLocation(m_shader->GetProgram(), "gMVP");
		if(worldLocation == -1)
		{
			std::cout << "Couldn't get uniform loaction" << std::endl;
		}
		glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(mvp));

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		if(m_display->GetInputModule()->IsKeyPressed(Key_Space))
		{
			glDrawArrays(GL_POINTS, 0, 4);
		}

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glDisableVertexAttribArray(0);
	}

private:
    GLuint m_VBO;
	GLuint m_IBO;
	glm::vec3* m_verts;
	uint* m_indices;
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

	RenderChain* renderChain = new RenderChain();
	if(!renderChain->InitRenderChain(10))
	{
		std::cout << "Couldn't init render chain" << std::endl;
		return 1;
	}

	ShittyObject* obj = new ShittyObject();
	if(!obj->InitObject(display))
	{
		std::cout << "Couldn't init shitty object" << std::endl;
		return 1;
	}

	//obj->Scale(glm::scale(glm::vec3(0.5f, 1.0f, 1.0f)));
	//obj->Rotate(glm::rotate(glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)));
	//obj->Translate(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

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

		if(display->GetInputModule()->IsKeyPressed(Key_W)) {
			display->GetCameraModule()->MoveCamera(glm::vec3(0.0f, 0.0f, 0.1f));
		}
		if(display->GetInputModule()->IsKeyPressed(Key_S)) {
			display->GetCameraModule()->MoveCamera(glm::vec3(0.0f, 0.0f, -0.1f));
		}
		if(display->GetInputModule()->IsKeyPressed(Key_A)) {
			display->GetCameraModule()->MoveCamera(glm::vec3(0.1f, 0.0f, 0.0f));
		}
		if(display->GetInputModule()->IsKeyPressed(Key_D)) {
			display->GetCameraModule()->MoveCamera(glm::vec3(-0.1f, 0.0f, 0.0f));
		}

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
