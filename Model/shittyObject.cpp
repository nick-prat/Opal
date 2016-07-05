//
// Created by nprat on 7/4/16.
//

#include <iostream>

#include "shittyObject.h"

ShittyObject::ShittyObject()
{
    m_VBO = 0;
    m_IBO = 0;
}

ShittyObject::ShittyObject(std::shared_ptr<Display> display)
{
    m_VBO = 0;
    m_IBO = 0;

    if(!InitObject(display))
    {
        throw "Couldn't init shitty model";
    }
}

ShittyObject::~ShittyObject()
{
    Destroy();
}

bool ShittyObject::InitObject(std::shared_ptr<Display> display)
{
    m_display = display;

    m_verts.reserve(4);
    m_verts[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
    m_verts[1] = glm::vec3(1.0f, -1.0f, 0.0f);
    m_verts[2] = glm::vec3(-1.0f, 1.0f, 0.0f);
    m_verts[3] = glm::vec3(1.0f, 1.0f, 0.0f);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 4, m_verts.data(), GL_STATIC_DRAW);

    m_indices.reserve(6);
    m_indices[0] = 0;
    m_indices[1] = 1;
    m_indices[2] = 2;
    m_indices[3] = 1;
    m_indices[4] = 3;
    m_indices[5] = 2;

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, m_indices.data(), GL_STATIC_DRAW);

    m_shader = std::make_unique<Shader>();
    //std::vector<std::string> files = {"Shaders/shader.vs", "Shaders/shader.fs"};
    //std::vector<GLenum> types = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
    std::vector<std::string> files = {"Shaders/shader.vs", "Shaders/shader.fs", "Shaders/shader.gs"};
    std::vector<GLenum> types = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER};
    if(!m_shader->InitShader(files, types))
    {
        std::cout << "Couldn't initialize shader" << std::endl;
        return false;
    }
    return true;
}

void ShittyObject::Destroy()
{
    glDeleteBuffers(1, &m_VBO);
    m_VBO = 0;
    glDeleteBuffers(1, &m_IBO);
    m_IBO = 0;
}

void ShittyObject::Render()
{
    m_shader->UseShader();

    glm::mat4 mvp = m_display->GetProjectionMatrix() * m_display->GetCameraModule()->GetViewMatrix() * GetWorld();

    GLint worldLocation = glGetUniformLocation(m_shader->GetProgram(), "gMVP");
    if(worldLocation == -1)
    {
        std::cout << "Couldn't get uniform loaction" << std::endl;
    }
    glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(mvp));

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_POINTS, 0, 4);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glDisableVertexAttribArray(0);
}