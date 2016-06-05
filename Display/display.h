#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum InputKey
{
	Key_W = 119,
	Key_A = 97,
	Key_S = 115,
	Key_D = 100,
	Key_Space = 32
};

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
		
		bool IsKeyPressed(InputKey key) const;
		glm::vec2 GetMouseLocation() const;
		void UpdateKey(int key, bool pressed);
		
	private:
		bool m_keys[256];
	};

	class CameraModule
	{
	public:

		CameraModule();
		~CameraModule();

		bool InitCamera();
		void Destroy();

		glm::mat4 GetViewMatrix();
		void MoveCamera(glm::vec3 delta);
		void RotateCamera(glm::vec3 rotation);

	protected:

	private:
		glm::mat4 m_viewMatrix;

		glm::vec3 m_translation;
		glm::vec3 m_rotation;
	};
	
    bool InitDisplay(int width, int height, std::string title);
    void Destroy();

    void Update();
    bool IsClosed();

	InputModule* GetInputModule();
	CameraModule* GetCameraModule();

	glm::mat4 GetProjectionMatrix();

private:
	InputModule* m_inputModule;
	CameraModule* m_cameraModule;
    SDL_Window* m_window;
    SDL_GLContext m_glcontext;
	glm::mat4 m_projMatrix;
    bool m_isClosed;
};

#endif // _DISPLAY_H
