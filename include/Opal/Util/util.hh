#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <Opal/Util/exceptions.hh>
#include <vector>
#include <string>
#include <fstream>
#include <assimp/matrix4x4.h>
#include <glm/glm.hpp>

namespace Opal::Util {

    using size_t = uint32_t;

    constexpr char OPLTAG[3]{'\x4F', '\x50', '\x4C'};
    constexpr unsigned char RES_TEXTURE_BPP = 4;
    constexpr unsigned char RES_HEIGHTMAP_BPP = 1;
    constexpr unsigned short VERSION = 1;

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

    template<typename T, typename U = void, typename... Ts>
    static constexpr unsigned int index(int i = 0) {
        if(std::is_same<T, U>::value) {
            return i;
        } else if (!std::is_same<U, void>::value) {
            return index<T, Ts...>(++i);
        } else {
            return -1;
        }
    }

    template<typename data_t>
    std::enable_if_t<!std::is_pointer<data_t>::value, void> write(std::ostream& stream, data_t data) {
        static_assert(std::is_standard_layout<data_t>(), "write failed, data isn't standard layout");
        stream.write((char*)&data, sizeof(std::decay_t<data_t>));
    }

    template<typename data_t>
    std::enable_if_t<!std::is_pointer<data_t>::value, data_t> read(std::istream& stream) {
        static_assert(std::is_standard_layout<data_t>(), "can't read data into non standard layout type");
        data_t data;
        stream.read((char*)&data, sizeof(data_t));
        return data;
    }

    template<typename data_t>
    std::enable_if_t<!std::is_pointer<data_t>::value, data_t> peek(std::istream& stream) {
        static_assert(std::is_standard_layout<data_t>(), "can't read data into non standard layout type");
        data_t data;
        auto byteSize{sizeof(data_t)};
        stream.read((char*)&data, sizeof(byteSize));
        stream.seekg(-byteSize, std::ios::cur);
        return data;
    }

    template<std::size_t length>
    bool bytecmp(const char* const l, const char* const r) {
        for(unsigned int i = 0; i < length; i++) {
            if(l[i] != r[i]) {
                return false;
            }
        }
        return true;
    }   
    
    uint32_t toRGB32(uint8_t const r, uint8_t const g, uint8_t const b);
    std::tuple<uint8_t, uint8_t, uint8_t> fromRGB32(uint32_t const rgb32);

    void PrintGLErrors();
    std::vector<std::string> splitString(const std::string& string, const char delim);
    void copyaiMat(const aiMatrix4x4* from, glm::mat4& to);
    void writeString(std::ostream& stream, const std::string& data);
    std::string readString(std::istream& stream);
    void printMatrix(const glm::mat4x4& matrix);

}

#endif // _UTILITIES_H
