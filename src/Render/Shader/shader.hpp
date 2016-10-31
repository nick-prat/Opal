#ifndef _SHADER_H
#define _SHADER_H

#include <vector>
#include <string>

#include <Core/glapi.hpp>

class Shader {
public:
    Shader();
    Shader(const std::vector<std::string>& fileNames, const std::vector<GLenum>& types);
    ~Shader();

    bool InitShader(const std::vector<std::string>& fileNames, const std::vector<GLenum>& types);
    void Destroy();

    void UseShader();
    GLuint GetProgram();

private:
    ulong m_numShaders;
    GLuint m_shaderProgram;
    std::vector<GLuint> m_shaderObj;
};

#endif // _SHADER_H
