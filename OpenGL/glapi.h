//
// Created by nprat on 7/5/16.
//

#ifndef OPENGL_GLAPI_H
#define OPENGL_GLAPI_H

#include <GL/gl.h>
#include <GL/glx.h>

namespace gl
{
    extern PFNGLGENBUFFERSPROC glGenBuffers;
    extern PFNGLVALIDATEPROGRAMPROC glValidateProgram;
    extern PFNGLUSEPROGRAMPROC glUseProgram;
    extern PFNGLCREATEPROGRAMPROC glCreateProgram;
    extern PFNGLCREATESHADERPROC glCreateShader;
    extern PFNGLSHADERSOURCEPROC glShaderSource;
    extern PFNGLCOMPILESHADERPROC glCompileShader;
    extern PFNGLGETSHADERIVPROC glGetShaderiv;
    extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
    extern PFNGLATTACHSHADERPROC glAttachShader;
    extern PFNGLLINKPROGRAMPROC glLinkProgram;
    extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
    extern PFNGLDETACHSHADERPROC glDetachShader;
    extern PFNGLDELETESHADERPROC glDeleteShader;
    extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
    extern PFNGLBINDBUFFERPROC glBindBuffer;
    extern PFNGLBUFFERDATAPROC glBufferData;
    extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
    extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
    extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
    extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
    extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
    extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
    extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
    extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

    void InitAPI();
}

#endif //OPENGL_GLAPI_H
