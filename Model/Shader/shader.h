#ifndef _SHADER_H
#define _SHADER_H

#include <vector>
#include <string>

#include <OpenGL/GLApi.h>

class Shader
{
public:
    Shader();
    ~Shader();

    bool InitShader(const std::vector<std::string>& filesNames, const std::vector<GLenum>& types);
    void Destroy();

    void UseShader();
    GLuint GetProgram();

private:
    GLuint m_shaderProgram;
    std::vector<GLuint> m_shaderObj;
};

#endif // _SHADER_H
