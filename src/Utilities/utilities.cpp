#include "utilities.hpp"

#include <sstream>
#include <iostream>

#include <GL/gl3w.h>

void Utilities::PrintGLErrors()
{
    for(;;)
    {
        GLenum glErr = glGetError();
        if(glErr != GL_NO_ERROR)
        {
            std::cout << "~~  GLError (" << glErr << ") " << glErr << " ~~" << std::endl;
        }
        else
        {
            break;
        }
    }
}
