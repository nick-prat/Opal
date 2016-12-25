#ifndef _SHADER_H
#define _SHADER_H

#include <vector>
#include <string>

#include <GL/gl3w.h>

class Shader {
public:
    Shader(std::vector<std::string>& fileNames, const std::vector<GLenum>& types);
    ~Shader();

    void useShader();
    GLuint getProgram();

private:
    ulong m_numShaders;
    GLuint m_shaderProgram;
    std::vector<GLuint> m_shaderObj;
};

#endif // _SHADER_H
