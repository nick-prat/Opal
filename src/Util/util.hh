#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <Util/exceptions.hh>
#include <vector>
#include <string>
#include <fstream>
#include <assimp/matrix4x4.h>
#include <glm/glm.hpp>

namespace Opal::Util {

    void PrintGLErrors();

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

    constexpr char OPLTAG[3]{'\x4F', '\x50', '\x4C'};

    enum ResType : unsigned char {
        Model3D = 0x01,
        Texture,
        Shader,
        ShaderVert,
        ShaderGeo,
        ShaderTessCtrl,
        ShaderTessEval,
        ShaderFrag,
        ShaderComp
    };

    constexpr unsigned char RES_TEXTURE_BPP = 4;
    constexpr unsigned short VERSION = 1;

    std::vector<std::string> splitString(const std::string &string, const char delim);

    void copyaiMat(const aiMatrix4x4* from, glm::mat4 &to);

    template<typename data_t>
    std::enable_if_t<!std::is_pointer<data_t>::value, void> write(std::ostream &stream, data_t data) {
        static_assert(std::is_standard_layout<data_t>(), "write failed, data isn't standard layout");
        stream.write((char*)&data, sizeof(std::decay_t<data_t>));
    }

    template<typename data_t>
    std::enable_if_t<std::is_pointer<data_t>::value, void> write(std::ostream &stream, data_t data) {
        static_assert(std::is_standard_layout<std::decay<data_t>>(), "write failed, data isn't standard layout");
        stream.write(data, sizeof(std::decay_t<data_t>));
    }

    template<>
    void write<const std::string&>(std::ostream &stream, const std::string &data) {
        std::vector<char> vec(data.begin(), data.end());
        stream.write(vec.data(), vec.size());
        stream.write("\0", sizeof(char));
    }

    template<typename data_t>
    std::enable_if_t<std::is_pointer<data_t>::value, void> write(std::ostream &stream, data_t data, std::size_t size) {
        static_assert(std::is_standard_layout<std::decay<data_t>>(), "write failed, data isn't standard layout");
        stream.write((char*)data, size);
    }

    template<typename data_t>
    std::enable_if_t<!std::is_pointer<data_t>::value, data_t> read(std::istream &stream) {
        static_assert(std::is_standard_layout<data_t>(), "read failed, data isn't standard layout");
        data_t data;
        stream.read((char*)&data, sizeof(std::decay_t<data_t>));
        return data;
    }

    template<>
    std::string read<std::string>(std::istream &stream) {
        std::vector<char> vec;
        char in = '\0';
        while(true) {
            stream.read(&in, sizeof(char));
            if(in == '\0') {
                break;
            }
            vec.push_back(in);
        }
        return std::string(vec.begin(), vec.end());
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

}

#endif // _UTILITIES_H
