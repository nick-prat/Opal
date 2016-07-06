//
// Created by nprat on 7/5/16.
//

#ifndef OPENGL_GLAPI_H
#define OPENGL_GLAPI_H

#include <GL/gl.h>

namespace gl
{
    extern "C"
    {
        PFNGLGENBUFFERSPROC glGenBuffers;
        PFNGLVALIDATEPROGRAMPROC glValidateProgram;
        PFNGLUSEPROGRAMPROC glUseProgram;
        PFNGLCREATEPROGRAMPROC glCreateProgram;
        PFNGLCREATESHADERPROC glCreateShader;
        PFNGLSHADERSOURCEPROC glShaderSource;
        PFNGLCOMPILESHADERPROC glCompileShader;
        PFNGLGETSHADERIVPROC glGetShaderiv;
        PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
        PFNGLATTACHSHADERPROC glAttachShader;
    }

    void InitAPI();
}

#endif //OPENGL_GLAPI_H
