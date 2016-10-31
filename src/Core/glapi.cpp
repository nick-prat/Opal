#include <iostream>

#include "glapi.hpp"

namespace gl
{
    PFNGLVALIDATEPROGRAMPROC glValidateProgram = 0;
    PFNGLUSEPROGRAMPROC glUseProgram = 0;
    PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
    PFNGLLINKPROGRAMPROC glLinkProgram = 0;
    PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = 0;
    PFNGLDELETEPROGRAMPROC glDeleteProgram = 0;
    PFNGLGETPROGRAMIVPROC glGetProgramiv = 0;

    PFNGLCREATESHADERPROC glCreateShader = 0;
    PFNGLSHADERSOURCEPROC glShaderSource = 0;
    PFNGLCOMPILESHADERPROC glCompileShader = 0;
    PFNGLGETSHADERIVPROC glGetShaderiv = 0;
    PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;
    PFNGLATTACHSHADERPROC glAttachShader = 0;
    PFNGLDETACHSHADERPROC glDetachShader = 0;
    PFNGLDELETESHADERPROC glDeleteShader = 0;

    PFNGLGENBUFFERSPROC glGenBuffers = 0;
    PFNGLBINDBUFFERPROC glBindBuffer = 0;
    PFNGLBUFFERDATAPROC glBufferData = 0;
    PFNGLDELETEBUFFERSPROC glDeleteBuffers = 0;

    PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
    PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = 0;
    PFNGLUNIFORM1IPROC glUniform1i = 0;

    PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
    PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;
    PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = 0;
    PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = 0;

    PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = 0;
    PFNGLBINDVERTEXARRAYPROC glBindVertexArray = 0;
    PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = 0;

    PFNGLGENSAMPLERSPROC glGenSamplers = 0;
    PFNGLDELETESAMPLERSPROC glDeleteSamplers = 0;
    PFNGLBINDSAMPLERPROC glBindSampler = 0;
    PFNGLBINDSAMPLERSPROC glBindSamplers = 0;
    PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = 0;

    PFNGLGENERATEMIPMAPPROC glGenerateMipmap = 0;

    void InitAPI()
    {
        glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glValidateProgram");
        glUseProgram = (PFNGLUSEPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glUseProgram");
        glCreateProgram = (PFNGLCREATEPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glCreateProgram");
        glLinkProgram = (PFNGLLINKPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glLinkProgram");
        glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glXGetProcAddress((const GLubyte*)"glGetProgramInfoLog");
        glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glDeleteProgram");
        glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glXGetProcAddress((const GLubyte*)"glGetProgramiv");

        glCreateShader = (PFNGLCREATESHADERPROC)glXGetProcAddress((const GLubyte*)"glCreateShader");
        glShaderSource = (PFNGLSHADERSOURCEPROC)glXGetProcAddress((const GLubyte*)"glShaderSource");
        glCompileShader = (PFNGLCOMPILESHADERPROC)glXGetProcAddress((const GLubyte*)"glCompileShader");
        glGetShaderiv = (PFNGLGETSHADERIVPROC)glXGetProcAddress((const GLubyte*)"glGetShaderiv");
        glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glXGetProcAddress((const GLubyte*)"glGetShaderInfoLog");
        glAttachShader = (PFNGLATTACHSHADERPROC)glXGetProcAddress((const GLubyte*)"glAttachShader");
        glDetachShader = (PFNGLDETACHSHADERPROC)glXGetProcAddress((const GLubyte*)"glDetachShader");
        glDeleteShader = (PFNGLDELETESHADERPROC)glXGetProcAddress((const GLubyte*)"glDeleteShader");

        glGenBuffers = (PFNGLGENBUFFERSPROC)glXGetProcAddress((const GLubyte*)"glGenBuffers");
        glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddress((const GLubyte*)"glBindBuffer");
        glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddress((const GLubyte*)"glBufferData");
        glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glXGetProcAddress((const GLubyte*)"glDeleteBuffers");

        glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glXGetProcAddress((const GLubyte*)"glGetUniformLocation");
        glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glXGetProcAddress((const GLubyte*)"glUniformMatrix4fv");
        glUniform1i = (PFNGLUNIFORM1IPROC)glXGetProcAddress((const GLubyte*)"glUniform1i");

        glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((const GLubyte*)"glEnableVertexAttribArray");
        glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddress((const GLubyte*)"glVertexAttribPointer");
        glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((const GLubyte*)"glDisableVertexAttribArray");
        glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)glXGetProcAddress((const GLubyte*)"glBindAttribLocation");

        glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress((const GLubyte*)"glGenVertexArrays");
        glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glXGetProcAddress((const GLubyte*)"glBindVertexArray");
        glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glXGetProcAddress((const GLubyte*)"glDeleteVertexArrays");

        glGenSamplers = (PFNGLGENSAMPLERSPROC)glXGetProcAddress((const GLubyte*)"glGenSamplers");
        glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)glXGetProcAddress((const GLubyte*)"glDeleteSamplers");
        glBindSampler = (PFNGLBINDSAMPLERPROC)glXGetProcAddress((const GLubyte*)"glBindSampler");
        glBindSamplers = (PFNGLBINDSAMPLERSPROC)glXGetProcAddress((const GLubyte*)"glBindSamplers");
        glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)glXGetProcAddress((const GLubyte*)"glSamplerParameteri");

        glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)glXGetProcAddress((const GLubyte*)"glGenerateMipmap");
    }
}
