#ifndef _SHADER_H
#define _SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <string>

class Shader
{
public:
    Shader();
    ~Shader();

    bool InitShader(std::vector<std::string> filesNames, std::vector<GLenum> types);
    void Destroy();
	
	void UseShader();
	GLuint GetProgram();

private:
	GLuint m_shaderProgram;
	std::vector<GLuint> m_shaderObj;
};

#endif // _SHADER_H
