#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <iostream>
#include <glm/glm.hpp>
#include <exception>
#include <vector>
#include <string>

#include "log.hpp"

typedef unsigned int uint;

namespace Utilities {
    void PrintGLErrors();

    class Exception : public std::exception {
    public:
        Exception(const std::string& error);
        ~Exception();

        void PrintError() const;
        std::string GetError() const;

        const char* what() const noexcept override;

    private:
        std::string error;
        int code;
    };

}

#endif // _UTILITIES_H
