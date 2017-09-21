#include <Opal/Core/gl.hh>
#include <Opal/Render/shader.hh>
#include <Opal/Util/exceptions.hh>
#include <Opal/Util/log.hh>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <list>

Opal::Shader::Shader(const Resources::RShader& shader)
: m_shaderName(shader.name)
, m_numShaders(0)
, m_shaderProgram(0) {
    GLint success;
    GLchar info[1024];
    m_numShaders = shader.files.size();
    m_shaderProgram = glCreateProgram();

    std::vector<GLuint> shaderObj;
    shaderObj.reserve(m_numShaders);
    for(auto& [type, file] : shader.files) {
        switch(type) {
        case Resources::ResType::ShaderFrag:
            shaderObj.push_back(glCreateShader(GL_FRAGMENT_SHADER));
            break;
        case Resources::ResType::ShaderGeo:
            shaderObj.push_back(glCreateShader(GL_GEOMETRY_SHADER));
            break;
        case Resources::ResType::ShaderVert:
            shaderObj.push_back(glCreateShader(GL_VERTEX_SHADER));
            break;
        default:
            throw std::runtime_error("Unknown shader type");
        }

        auto& shader = shaderObj.back();
        auto dataPtr = file.bytes.data();
        int length = file.bytes.size();

        glShaderSource(shader, 1, (const GLchar *const *) &dataPtr, &length);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(success == GL_FALSE) {
            glGetShaderInfoLog(shader, sizeof(info), nullptr, info);
            throw GenericException(info);
        }

        glAttachShader(m_shaderProgram, shader);
    }

    glLinkProgram(m_shaderProgram);
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if(success == GL_FALSE) {
        glGetProgramInfoLog(m_shaderProgram, sizeof(info), nullptr, info);
        throw GenericException(std::string("Shader IV didn't succeed ") + info);
    }

    for(auto i{0u}; i < m_numShaders; i++) {
        glDetachShader(m_shaderProgram, shaderObj[i]);
        glDeleteShader(shaderObj[i]);
    }

    glValidateProgram(m_shaderProgram);
    GLint status = 0;
    glGetProgramiv(m_shaderProgram, GL_VALIDATE_STATUS, &status);
    if(status != GL_TRUE) {
        throw GenericException("Couldn't validate program");
    }

    for(const auto& uniform : shader.uniforms) {
        registerUniform(uniform);
    }
}

Opal::Shader::Shader(Shader&& shader)
: m_numShaders(shader.m_numShaders)
, m_shaderProgram(shader.m_shaderProgram)
, m_uniformLocations(std::move(shader.m_uniformLocations)) {
    shader.m_numShaders = 0;
    shader.m_shaderProgram = 0;
}

Opal::Shader::~Shader() {
    if(glIsProgram(m_shaderProgram)) {
        glDeleteProgram(m_shaderProgram);
    }
}

void Opal::Shader::registerUniform(const std::string& name) {
    auto loc{glGetUniformLocation(m_shaderProgram, name.c_str())};
    if(loc != -1) {
        m_uniformLocations[name] = loc;
    } else {
        Log::getErrorLog<SyncLogger>() << "uniform " << name << " not found for shader " << m_shaderName << '\n';
    }
}

GLint Opal::Shader::getUniformLocation(const std::string& name) const {
    if(auto uniform = m_uniformLocations.find(name); uniform != m_uniformLocations.end()) {
        return uniform->second;
    } else {
        Log::getErrorLog<SyncLogger>() << "uniform " << name << " not found for shader " << m_shaderName << '\n';
        return -1;
    }
}

void Opal::Shader::useProgram() const {
    glUseProgram(m_shaderProgram);
}

GLuint Opal::Shader::getProgram() const {
    return m_shaderProgram;
}
