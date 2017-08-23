#ifndef _SHADER_H
#define _SHADER_H

#include <Opal/Core/gl.hh>
#include <Opal/Resources/resources.hh>

#include <vector>
#include <string>
#include <list>
#include <unordered_map>

namespace Opal {

    // NOTE What else are shader's capable of? Do i need to implement more functions in this wrapper?
    // TODO Implement lighting in shaders
    class Shader {
    public:
        // Shader(std::vector<std::string> fileNames, const std::vector<GLenum>& types);
        Shader(const Shader&) = delete;
        Shader(Shader&& shader);
        Shader(const Resources::RShader& shader);
        ~Shader();

        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&&) = delete;

        void registerUniform(const std::string& name);
        GLint getUniformLocation(const std::string& name) const;

        void useProgram() const;
        GLuint getProgram() const;

    private:
        ulong m_numShaders;
        GLuint m_shaderProgram;
        std::unordered_map<std::string, GLint> m_uniformLocations;
    };

}

#endif // _SHADER_H
