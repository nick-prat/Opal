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

bool Display::InputModule::IsKeyPressed(InputKey key) const
{
	return m_keys[key];
}

glm::vec2 Display::InputModule::GetMouseLocation() const
{
	return glm::vec2(0.0f, 0.0f);
}

void Display::InputModule::UpdateKey(int key, bool pressed)
{
	if(key > 256 || key < 0)
	{
		return;
	}
	m_keys[key] = pressed;
}