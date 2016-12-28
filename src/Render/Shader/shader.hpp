#ifndef _SHADER_H
#define _SHADER_H

#include <vector>
#include <string>
#include <unordered_map>

#include <GL/gl3w.h>

class Shader {
public:
    Shader(std::vector<std::string>& fileNames, const std::vector<GLenum>& types);
    ~Shader();

    void useShader();
    void registerUniform(const std::string& name);

    GLint getUniformLocation(const std::string& name);
    GLuint getProgram();

private:
    ulong m_numShaders;
    GLuint m_shaderProgram;

    std::unordered_map<std::string, GLint> m_uniformLocations;
    std::vector<GLuint> m_shaderObj;
};

#endif // _SHADER_H
