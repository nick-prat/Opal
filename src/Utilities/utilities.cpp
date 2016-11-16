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
