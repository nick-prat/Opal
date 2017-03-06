#include "utilities.hpp"

#include <GL/gl3w.h>

#include <Utilities/log.hpp>

void Utilities::PrintGLErrors()
{
    for(GLenum glErr = glGetError(); glErr != GL_NO_ERROR; glErr = glGetError())
    {
        Log::getErrorLog() << "~~  GLError (" << glErr << ") " << glErr << " ~~\n";
    }
}
