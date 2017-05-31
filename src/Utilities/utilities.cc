#include "utilities.hh"

#include <GL/gl3w.h>

#include <Utilities/log.hh>

void Utilities::PrintGLErrors()
{
    for(GLenum glErr = glGetError(); glErr != GL_NO_ERROR; glErr = glGetError())
    {
        Log::getErrorLog() << "~~  GLError (" << glErr << ") " << glErr << " ~~\n";
    }
}
