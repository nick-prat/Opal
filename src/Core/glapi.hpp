#ifndef OPENGL_GLAPI_H
#define OPENGL_GLAPI_H

#include <GL/gl.h>
#include <GL/glx.h>

namespace gl
{
    extern PFNGLVALIDATEPROGRAMPROC glValidateProgram;
    extern PFNGLUSEPROGRAMPROC glUseProgram;
    extern PFNGLCREATEPROGRAMPROC glCreateProgram;
    extern PFNGLLINKPROGRAMPROC glLinkProgram;
    extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
    extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
    extern PFNGLGETPROGRAMIVPROC glGetProgramiv;

    extern PFNGLCREATESHADERPROC glCreateShader;
    extern PFNGLSHADERSOURCEPROC glShaderSource;
    extern PFNGLCOMPILESHADERPROC glCompileShader;
    extern PFNGLGETSHADERIVPROC glGetShaderiv;
    extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
    extern PFNGLATTACHSHADERPROC glAttachShader;
    extern PFNGLDETACHSHADERPROC glDetachShader;
    extern PFNGLDELETESHADERPROC glDeleteShader;

    extern PFNGLGENBUFFERSPROC glGenBuffers;
    extern PFNGLBINDBUFFERPROC glBindBuffer;
    extern PFNGLBUFFERDATAPROC glBufferData;
    extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;

    extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
    extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
    extern PFNGLUNIFORM1IPROC glUniform1i;

    extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
    extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
    extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
    extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;

    extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
    extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
    extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

    extern PFNGLGENSAMPLERSPROC glGenSamplers;
    extern PFNGLDELETESAMPLERSPROC glDeleteSamplers;
    extern PFNGLBINDSAMPLERPROC glBindSampler;
    extern PFNGLBINDSAMPLERSPROC glBindSamplers;
    extern PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri;

    void InitAPI();
}

#endif //OPENGL_GLAPI_H
