#ifndef _SHADER_H
#define _SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>

class Shader
{
public:
    Shader();
    ~Shader();

    bool InitShader(GLenum shaderType);
    void Destroy();

private:
	GLuint m_shaderProgram;
	GLuint m_shaderObj;
};

#endif // _SHADER_H
