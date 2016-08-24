#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <iostream>
#include <glm/glm.hpp>

#include "log.h"

typedef unsigned int uint;

namespace Utilities
{
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
