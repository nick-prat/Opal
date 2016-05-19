#include <iostream>
#include <GL/glew.h>
#include "display.h"

using std::cout;
using std::cerr;
using std::endl;

Display::Display()
{
    m_window = nullptr;
}


Display::~Display()
{

}

bool Display::InitDisplay(int width, int height, std::string title)
{
    SDL_Init(SDL_INIT_EVERYTHING);

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
        cerr << "Glew failed to initialize: " << status << endl;
        return false;
    }

    m_isClosed = false;

    return true;
}

void Display::Destroy()
{
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
        if(event.type == SDL_QUIT)
        {
            m_isClosed = true;
        }
    }
}

bool Display::IsClosed()
{
    return m_isClosed;
}

