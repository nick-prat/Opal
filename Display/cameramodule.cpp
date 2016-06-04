#include "display.h"

Display::CameraModule::CameraModule()
{
    m_viewMatrix = glm::mat4(1.0f);
}

Display::CameraModule::~CameraModule()
{

}

bool Display::CameraModule::InitCamera()
{
    m_viewMatrix = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 2.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));

    return true;
}

void Display::CameraModule::Destroy()
{

}

glm::mat4 Display::CameraModule::GetViewMatrix()
{
    //glm::mat4 view = m_viewMatrix *

    return m_viewMatrix;
}

void Display::CameraModule::RotateCamera(glm::mat4 rotation)
{

}

void Display::CameraModule::MoveCamera(glm::vec3 delta)
{

}
