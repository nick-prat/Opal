#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Utilities/utilities.hpp"
#include "Display/display.h"

using Utilities::SafeDelete;

Display::Display()
{
    m_inputModule = nullptr;
    m_isClosed = false;
    m_projMatrix = glm::mat4(1.0f);
}

Display::Display(int width, int height, std::string title)
{
    m_isClosed = false;
    m_projMatrix = glm::mat4(1.0f);

    if(!InitDisplay(width, height, title))
    {
        throw "Couldn't init display";
    }
}

Display::~Display()
{
    Destroy();
}

glm::mat4 Display::GetProjectionMatrix()
{
    return m_projMatrix;
}

void callback()
{

}

bool Display::InitDisplay(int width, int height, std::string title)
{

    m_projMatrix = glm::perspective(glm::radians(75.0f), (float) width / (float) height, 0.1f, 100.0f);
    m_inputModule = std::make_shared<InputModule>();
    m_cameraModule = std::make_shared<CameraModule>();

    return true;
}

void Display::Destroy()
{

}

void Display::Update()
{

}

std::shared_ptr<Display::InputModule> Display::GetInputModule()
{
    return m_inputModule;
}

std::shared_ptr<Display::CameraModule> Display::GetCameraModule()
{
    return m_cameraModule;
}

bool Display::IsClosed()
{
    return m_isClosed;
}



