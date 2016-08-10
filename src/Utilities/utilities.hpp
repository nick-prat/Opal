#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <iostream>
#include <glm/glm.hpp>

typedef unsigned int uint;

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