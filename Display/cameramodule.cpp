#include <iostream>
#include "display.h"

Display::CameraModule::CameraModule()
{
    m_viewMatrix = glm::mat4(1.0f);
    m_translation = glm::vec3(0.0f, 0.0f, 0.0f);
    m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    if(!InitCamera())
    {
        std::cout << "Couldn't initialize camera!" << std::endl;
        throw;
    }
}

Display::CameraModule::~CameraModule()
{
    Destroy();
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

    return m_viewMatrix * glm::translate(m_translation);
}

void Display::CameraModule::RotateCamera(glm::vec3 rotation)
{
    m_rotation = m_rotation + rotation;
}

void Display::CameraModule::MoveCamera(glm::vec3 delta)
{
    m_translation = m_translation + delta;
}
