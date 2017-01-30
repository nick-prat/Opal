#include "utilities.hpp"

#include <GL/gl3w.h>

#include <Utilities/log.hpp>

void Utilities::PrintGLErrors()
{
    for(;;)
    {
        GLenum glErr = glGetError();
        if(glErr != GL_NO_ERROR)
        {
            Log::getErrorLog() << "~~  GLError (" << glErr << ") " << glErr << " ~~\n";
        }
        else
        {
            break;
        }
    }
}
