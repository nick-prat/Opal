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

Utilities::Exception::Exception(const unsigned char code, const std::string error)
{
    this->code = code;
    this->error = error;
}

Utilities::Exception::~Exception()
{}

void Utilities::Exception::PrintError() const
{
    std::stringstream ss;
    ss << "(" << code << "): " << error << std::endl;
    Log::error(ss.str(), Log::OUT_LOG | Log::OUT_CONS);
}

void Utilities::Exception::SetCode(int code)
{
    this->code = code;
}

int Utilities::Exception::GetCode() const
{
    return this->code;
}

void Utilities::Exception::SetError(std::string error)
{
    this->error = error;
}

std::string Utilities::Exception::GetError() const
{
    return this->error;
}
