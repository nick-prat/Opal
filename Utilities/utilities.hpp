#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <iostream>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

namespace Utilities
{
    template<typename T>
    void SafeDelete(T* &object)
    {
        if(object != nullptr)
        {
            delete object;
            object = nullptr;
        }
    }
}

#endif // _UTILITIES_H