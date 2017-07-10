#include "utilities.hh"

#include <Core/gl.hh>

#include <Utilities/log.hh>

void Utilities::PrintGLErrors()
{
    for(GLenum glErr = glGetError(); glErr != GL_NO_ERROR; glErr = glGetError())
    {
        Log::getErrorLog<SyncLogger>() << "~~  GLError (" << glErr << ") " << glErr << " ~~\n";
    }
}
