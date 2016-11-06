#include "utilities.hpp"

#include <sstream>
#include <iostream>

#include <GL/glu.h>

void Utilities::PrintGLErrors()
{
    for(;;)
    {
        GLenum glErr = glGetError();
        if(glErr != GL_NO_ERROR)
        {
            std::cout << "~~  GLError (" << glErr << ") " << gluErrorString(glErr) << " ~~" << std::endl;
        }
        else
        {
            break;
        }
    }
}

Utilities::Exception::Exception(const std::string& error)
{
    m_error = error;
}

Utilities::Exception::~Exception()
{}

void Utilities::Exception::PrintError() const
{
    Log::error(m_error, Log::OUT_LOG | Log::OUT_CONS);
}

std::string Utilities::Exception::GetError() const
{
    return m_error;
}

const char* Utilities::Exception::what() const noexcept {
    return m_error.c_str();
}
