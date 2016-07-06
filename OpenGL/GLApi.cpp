//
// Created by nprat on 7/5/16.
//

#include <GL/glx.h>
#include <GL/glext.h>
#include <iostream>

#include "GLApi.h"

//PFNGLGENBUFFERSPROC gl::glGenBuffers = nullptr;

void gl::InitAPI()
{
    gl::glGenBuffers = nullptr;
    glGenBuffers = (PFNGLGENBUFFERSPROC)glXGetProcAddress((const GLubyte*)"glGenBuffers");
    if(glGenBuffers == nullptr)
    {
        std::cout << "Couldn't find glGenBuffers" << std::endl;
    }
}