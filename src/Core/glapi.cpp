#include <iostream>

#include "glapi.hpp"

namespace gl
{
    PFNGLGENBUFFERSPROC glGenBuffers = 0;
    PFNGLVALIDATEPROGRAMPROC glValidateProgram = 0;
    PFNGLUSEPROGRAMPROC glUseProgram = 0;
    PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
    PFNGLCREATESHADERPROC glCreateShader = 0;
    PFNGLSHADERSOURCEPROC glShaderSource = 0;
    PFNGLCOMPILESHADERPROC glCompileShader = 0;
    PFNGLGETSHADERIVPROC glGetShaderiv = 0;
    PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;
    PFNGLATTACHSHADERPROC glAttachShader = 0;
    PFNGLLINKPROGRAMPROC glLinkProgram = 0;
    PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = 0;
    PFNGLDETACHSHADERPROC glDetachShader = 0;
    PFNGLDELETESHADERPROC glDeleteShader = 0;
    PFNGLDELETEPROGRAMPROC glDeleteProgram = 0;
    PFNGLBINDBUFFERPROC glBindBuffer = 0;
    PFNGLBUFFERDATAPROC glBufferData = 0;
    PFNGLDELETEBUFFERSPROC glDeleteBuffers = 0;
    PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
    PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = 0;
    PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
    PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;
    PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = 0;
    PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = 0;
    PFNGLBINDVERTEXARRAYPROC glBindVertexArray = 0;
    PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = 0;
    PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = 0;
    PFNGLGENSAMPLERSPROC glGenSamplers = 0;
    PFNGLDELETESAMPLERSPROC glDeleteSamplers = 0;

    void InitAPI()
    {
        glGenBuffers = (PFNGLGENBUFFERSPROC)glXGetProcAddressARB((const GLubyte*)"glGenBuffers");
        glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)glXGetProcAddressARB((const GLubyte*)"glValidateProgram");
        glUseProgram = (PFNGLUSEPROGRAMPROC)glXGetProcAddressARB((const GLubyte*)"glUseProgram");
        glCreateProgram = (PFNGLCREATEPROGRAMPROC)glXGetProcAddressARB((const GLubyte*)"glCreateProgram");
        glCreateShader = (PFNGLCREATESHADERPROC)glXGetProcAddressARB((const GLubyte*)"glCreateShader");
        glShaderSource = (PFNGLSHADERSOURCEPROC)glXGetProcAddressARB((const GLubyte*)"glShaderSource");
        glCompileShader = (PFNGLCOMPILESHADERPROC)glXGetProcAddressARB((const GLubyte*)"glCompileShader");
        glGetShaderiv = (PFNGLGETSHADERIVPROC)glXGetProcAddressARB((const GLubyte*)"glGetShaderiv");
        glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glXGetProcAddressARB((const GLubyte*)"glGetShaderInfoLog");
        glAttachShader = (PFNGLATTACHSHADERPROC)glXGetProcAddressARB((const GLubyte*)"glAttachShader");
        glLinkProgram = (PFNGLLINKPROGRAMPROC)glXGetProcAddressARB((const GLubyte*)"glLinkProgram");
        glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glXGetProcAddressARB((const GLubyte*)"glGetProgramInfoLog");
        glDetachShader = (PFNGLDETACHSHADERPROC)glXGetProcAddressARB((const GLubyte*)"glDetachShader");
        glDeleteShader = (PFNGLDELETESHADERPROC)glXGetProcAddressARB((const GLubyte*)"glDeleteShader");
        glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glXGetProcAddressARB((const GLubyte*)"glDeleteProgram");
        glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddressARB((const GLubyte*)"glBindBuffer");
        glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddressARB((const GLubyte*)"glBufferData");
        glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glXGetProcAddressARB((const GLubyte*)"glDeleteBuffers");
        glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glXGetProcAddressARB((const GLubyte*)"glGetUniformLocation");
        glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glXGetProcAddressARB((const GLubyte*)"glUniformMatrix4fv");
        glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddressARB((const GLubyte*)"glEnableVertexAttribArray");
        glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddressARB((const GLubyte*)"glVertexAttribPointer");
        glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)glXGetProcAddressARB((const GLubyte*)"glDisableVertexAttribArray");
        glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddressARB((const GLubyte*)"glGenVertexArrays");
        glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glXGetProcAddressARB((const GLubyte*)"glBindVertexArray");
        glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glXGetProcAddressARB((const GLubyte*)"glDeleteVertexArrays");
        glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)glXGetProcAddressARB((const GLubyte*)"glBindAttribLocation");
        glGenSamplers = (PFNGLGENSAMPLERSPROC)glXGetProcAddress((const GLubyte*)"glGenSamplers");
        glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)glXGetProcAddress((const GLubyte*)"glDeleteSamplers");
    }
}
