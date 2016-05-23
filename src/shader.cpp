#include "shader.h"

Shader::Shader()
{

}

Shader::~Shader()
{

}

bool Shader::InitShader(GLenum shaderType)
{
	m_shaderProgram = glCreateProgram();
	m_shaderObj = glCreateShader(shaderType);
	
	
	
    return true;
}

void Shader::Destroy()
{

}
