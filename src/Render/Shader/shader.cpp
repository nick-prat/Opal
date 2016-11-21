#include "shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

#include <Utilities/exceptions.hpp>

Shader::Shader(std::vector<std::string>& fileNames, const std::vector<GLenum>& types)
        : m_numShaders(0), m_shaderProgram(0) {

    if(fileNames.size() != types.size() && fileNames.size() != 0 && types.size() != 0) {
        throw generic_exception("Couldn't initialize shader: incorrect information passed");
    }

    for(std::string& filename: fileNames) {
        filename = "./Shaders/" + filename;
    }

    GLint success;
    GLchar info[1024];
    m_numShaders = types.size();

    m_shaderProgram = glCreateProgram();
    for(unsigned int i = 0; i < m_numShaders; i++) {
        m_shaderObj.push_back(glCreateShader(types[i]));

        std::ifstream file(fileNames[i]);
        if(!file.is_open()) {
            throw generic_exception(std::string("Couldn't open file: ") + fileNames[i]);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        GLchar* text[1];
        text[0] = new GLchar[buffer.str().length() + 1];
        strcpy(text[0], buffer.str().c_str());
        GLint length[1];
        length[0]= (GLint)buffer.str().length();

        glShaderSource(m_shaderObj[i], 1, (const GLchar *const *) text, length);
        glCompileShader(m_shaderObj[i]);

        glGetShaderiv(m_shaderObj[i], GL_COMPILE_STATUS, &success);
        if(success == GL_FALSE) {
            glGetShaderInfoLog(m_shaderObj[i], sizeof(info), nullptr, info);
            throw generic_exception(info);
        }

        glAttachShader(m_shaderProgram, m_shaderObj[i]);

        delete[] text[0];
    }

    glLinkProgram(m_shaderProgram);
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if(success == GL_FALSE) {
        glGetProgramInfoLog(m_shaderProgram, sizeof(info), nullptr, info);
        throw generic_exception(std::string("Shader IV didn't succeed ") + info);
    }

    for(unsigned int i = 0; i < m_numShaders; i++) {
        glDetachShader(m_shaderProgram, m_shaderObj[i]);
        glDeleteShader(m_shaderObj[i]);
    }
}

Shader::~Shader() {}

void Shader::UseShader() {
    glValidateProgram(m_shaderProgram);
    GLint status = 0;
    glGetProgramiv(m_shaderProgram, GL_VALIDATE_STATUS, &status);

    if(status == GL_TRUE) {
        glUseProgram(m_shaderProgram);
    } else {
        throw generic_exception("Couldn't validate program");
    }
}

GLuint Shader::GetProgram() {
    return m_shaderProgram;
}
