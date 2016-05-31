#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Display
{

public:
    Display();
    ~Display();
	
	class InputModule
	{
	public:
		
		InputModule();
		~InputModule();
		
		bool IsKeyPressed(unsigned char key);
		glm::vec2 GetMouseLocation();
		
	private:
		bool m_keys[256];
	};
	
    bool InitDisplay(int width, int height, std::string title);
    void Destroy();

    void Update();
    bool IsClosed();
	
	glm::mat4 GetProjectionMatrix();

private:
	InputModule* m_inputModule;
    SDL_Window* m_window;
    SDL_GLContext m_glcontext;
	glm::mat4 m_projMatrix;
    bool m_isClosed;
};

#endif // _DISPLAY_H