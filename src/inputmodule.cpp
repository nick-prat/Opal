#include "display.h"

Display::InputModule::InputModule()
{
	for(int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}
}

Display::InputModule::~InputModule()
{
	
}

bool Display::InputModule::IsKeyPressed(unsigned char key)
{
	return m_keys[key];
}

glm::vec2 Display::InputModule::GetMouseLocation()
{
	return glm::vec2(0.0f, 0.0f);
}