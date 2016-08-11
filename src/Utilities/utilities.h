#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <iostream>
#include <glm/glm.hpp>

typedef unsigned int uint;

namespace Utilities
{
    class Exception
    {
    public:
        Exception(const unsigned char code, const std::string error);
        ~Exception();

        void PrintError();

        void SetCode(int code);
        int GetCode();

        void SetError(std::string error);
        std::string GetError();

    private:
        std::string error;
        int code;
    };

}

#endif // _UTILITIES_H
