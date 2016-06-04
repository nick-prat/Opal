#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

#include "Utilities/utilities.hpp"
#include "Display/display.h"

using Utilities::SafeDelete;

Display::Display()
{
    m_window = nullptr;
    m_glcontext = nullptr;
    m_inputModule = nullptr;
	m_isClosed = false;
	m_projMatrix = glm::mat4(1.0f);
}

Display::~Display()
{

}

glm::mat4 Display::GetProjectionMatrix()
{
	return m_projMatrix;
}

bool Display::InitDisplay(int width, int height, std::string title)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
	}

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
    m_window = SDL_CreateWindow(title.c_str(), 0, 0, width, height, SDL_WINDOW_OPENGL);
	
    m_glcontext = SDL_GL_CreateContext(m_window);

    glewExperimental = true;
    GLenum status = glewInit();

    if(status != GLEW_OK)
    {
        std::cout << "Glew failed to initialize: " << status << std::endl;
        return false;
    }

    m_projMatrix = glm::perspective(glm::radians(75.0f), (float) width / (float) height, 0.1f, 100.0f);

    m_inputModule = new InputModule();

    m_cameraModule = new CameraModule();
    if(!m_cameraModule->InitCamera())
    {
        std::cout << "Couldn't init camera" << std::endl;
        return false;
    }

    return true;
}

void Display::Destroy()
{
    m_cameraModule->Destroy();
    SafeDelete(m_cameraModule);
    m_cameraModule = nullptr;
    SafeDelete(m_inputModule);
    m_inputModule = nullptr;
    SDL_GL_DeleteContext(m_glcontext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Display::Update()
{
    SDL_GL_SwapWindow(m_window);
	
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
		switch(event.type)
		{
		case SDL_QUIT:
			m_isClosed = true;
			break;
		case SDL_KEYDOWN:
            m_inputModule->UpdateKey(event.key.keysym.sym, true);
			break;
		case SDL_KEYUP:
            m_inputModule->UpdateKey(event.key.keysym.sym, false);
			break;
        default:
            break;
		}
    }
}

Display::InputModule* Display::GetInputModule()
{
    return m_inputModule;
}

Display::CameraModule* Display::GetCameraModule()
{
    return m_cameraModule;
}

bool Display::IsClosed()
{
    return m_isClosed;
}

