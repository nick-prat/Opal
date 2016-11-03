#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <iostream>
#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "log.hpp"

typedef unsigned int uint;

namespace Utilities
{
    void PrintGLErrors();

    class Exception
    {
    public:
        Exception(const unsigned char code, const std::string error);
        ~Exception();

        void PrintError() const;

        void SetCode(int code);
        int GetCode() const;

        void SetError(std::string error);
        std::string GetError() const;

    private:
        std::string error;
        int code;
    };

}

#endif // _UTILITIES_H
