#ifndef _SHADER_H
#define _SHADER_H

#include <vector>
#include <string>

#include <Core/glapi.hpp>

class Shader
{
public:
    Shader();
    ~Shader();

    bool InitShader(const std::vector<std::string>& filesNames, const std::vector<GLenum>& types);
    bool LinkProgram();
    void Destroy();

    void BindAttribute(const char* name, GLuint location);

    void UseShader();
    GLuint GetProgram();

private:
    ulong m_numShaders;
    GLuint m_shaderProgram;
    std::vector<GLuint> m_shaderObj;
};

#endif // _SHADER_H
