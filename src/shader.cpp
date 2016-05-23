#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

#include "shader.h"

Shader::Shader()
{
		m_shaderProgram = 0;
}

Shader::~Shader()
{}

void Shader::UseShader()
{
	glValidateProgram(m_shaderProgram);
	glUseProgram(m_shaderProgram);
}

bool Shader::InitShader(std::vector<std::string> fileNames, std::vector<GLenum> types)
{
	if(fileNames.size() != types.size() && fileNames.size() != 0 && types.size() != 0)
	{
		std::cout << "Couldn't initialize shader: incorrect information passed" << std::endl;
		return false;
	}
	
	GLint success;
	GLchar info[1024];
	unsigned int size = types.size();
	m_shaderObj.reserve(size);
	
	m_shaderProgram = glCreateProgram();
	for(unsigned int i = 0; i < size; i++)
	{
		m_shaderObj[i] = glCreateShader(types[i]);
		
		std::ifstream file(fileNames[i]);
		if(!file.is_open())
		{
			std::cout << "Couldn't open file: " << fileNames[i] << std::endl;
			return false;
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		
		GLchar* text[1];
		text[0] = new GLchar[buffer.str().length() + 1];
		strcpy(text[0], buffer.str().c_str());
		GLint length[1];
		length[0]= buffer.str().length();
		
		glShaderSource(m_shaderObj[i], 1, text, length);
		glCompileShader(m_shaderObj[i]);
		
		glGetShaderiv(m_shaderObj[i], GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(m_shaderObj[i], sizeof(info), nullptr, info);
			std::cout << info << std::endl;
			return false;
		}
		
		glAttachShader(m_shaderProgram, m_shaderObj[i]);
	}
	
	glLinkProgram(m_shaderProgram);
	
	glGetShaderiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(m_shaderProgram, sizeof(info), nullptr, info);
		std::cout << info << std::endl;
		return false;
	}
	
	for(unsigned int i = 0; i < size; i++)
	{
		glDetachShader(m_shaderProgram, m_shaderObj[i]);
		glDeleteShader(m_shaderObj[i]);
	}
	
    return true;
}

void Shader::Destroy()
{
	glDeleteProgram(m_shaderProgram);
}

GLuint Shader::GetProgram()
{
	return m_shaderProgram;
}