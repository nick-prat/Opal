#ifndef _SHADER_H
#define _SHADER_H

#include <vector>
#include <string>
#include <unordered_map>

#include <Resources/resource.hpp>

#include <GL/gl3w.h>

// NOTE What else are shader's capable of? Do i need to implement more functions in this wrapper?
// TODO Implement lighting in shaders

class Shader {
public:
    Shader(std::vector<std::string>& fileNames, const std::vector<GLenum>& types);
    ~Shader();

    void useShader();
    void registerUniform(const std::string& name);

    GLint getUniformLocation(const std::string& name) const;
    GLuint getProgram() const;

private:
    ulong m_numShaders;
    GLuint m_shaderProgram;

    std::unordered_map<std::string, GLint> m_uniformLocations;
    std::vector<GLuint> m_shaderObj;
};

#endif // _SHADER_H
