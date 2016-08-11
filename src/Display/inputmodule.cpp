#include "display.h"

GlutDisplay::InputModule::InputModule()
{
	for(int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}
}

GlutDisplay::InputModule::~InputModule()
{

}

bool GlutDisplay::InputModule::IsKeyPressed(unsigned char key) const
{
	return m_keys[key];
}

glm::vec2 GlutDisplay::InputModule::GetMouseLocation() const
{
	return glm::vec2(0.0f, 0.0f);
}

void GlutDisplay::InputModule::UpdateKey(unsigned char key, bool pressed)
{
	m_keys[key] = pressed;
}
