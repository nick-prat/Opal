#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <Utilities/exceptions.hpp>

// NOTE This is lackluster, is it worth having?

namespace Utilities {

    void PrintGLErrors();

    template<typename T, typename U = void, typename... Ts>
    static constexpr unsigned int index(int i = 0) {
        if(std::is_same<U, void>::value) {
            throw GenericException("Index couldn't be found");
        } else if(std::is_same<T, U>::value) {
            return i;
        } else {
            return index<T, Ts...>(++i);
        }
    }

    template<typename T, typename U = void, typename... Ts>
    static constexpr bool contains() {
        if(std::is_same<U, void>::value) {
            return false;
        } else if(std::is_same<T, U>::value) {
            return true;
        } else {
            return contains<T, Ts...>();
        }
    }

}

#endif // _UTILITIES_H
