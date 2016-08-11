//
// Created by nprat on 7/4/16.
//

#include <iostream>

#include "testObject.h"

TestObject::TestObject()
{
    m_IBO = 0;
}

TestObject::TestObject(std::shared_ptr<GlutDisplay> display)
{
    m_IBO = 0;

    if(!InitObject(display))
    {
        throw "Couldn't init shitty model";
    }
}

TestObject::~TestObject()
{
    Destroy();
}

bool TestObject::InitObject(std::shared_ptr<GlutDisplay> display)
{
    m_display = display;

    gl::glGenVertexArrays(1, &m_VAO);
    gl::glBindVertexArray(m_VAO);

    gl::glGenBuffers(2, m_VBO);

    m_verts.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
    m_verts.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
    m_verts.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));

    gl::glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
    gl::glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_verts.size(), m_verts.data(), GL_STATIC_DRAW);
    gl::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    m_colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    m_colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    m_colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

    gl::glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
    gl::glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_colors.size(), m_colors.data(), GL_STATIC_DRAW);
    gl::glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    m_indices.push_back(2);
    m_indices.push_back(1);
    m_indices.push_back(0);

    gl::glGenBuffers(1, &m_IBO);
    gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    gl::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

    m_shader = std::make_unique<Shader>();
    std::vector<std::string> files = {"src/Shaders/shader.vs", "src/Shaders/shader.fs"};
    std::vector<GLenum> types = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};

    // Use the geometry shader
    //std::vector<std::string> files = {"src/Shaders/shader.vs", "src/Shaders/shader.fs", "src/Shaders/shader.gs"};
    //std::vector<GLenum> types = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER};

    if(!m_shader->InitShader(files, types))
    {
        std::cout << "Couldn't initialize shader" << std::endl;
        return false;
    }

    m_shader->BindAttribute("Position", 0);
    m_shader->BindAttribute("Color", 1);

    if(!m_shader->LinkProgram())
    {
        std::cout << "Couldn't link shader" << std::endl;
        return false;
    }

    return true;
}

void TestObject::Destroy()
{
    gl::glDeleteVertexArrays(1, &m_VAO);
    gl::glDeleteBuffers(2, m_VBO);
    gl::glDeleteBuffers(1, &m_IBO);
    m_IBO = 0;
}

void TestObject::Render()
{
    m_shader->UseShader();

    glm::mat4 mvp = m_display->GetProjectionMatrix() * m_display->GetCameraModule()->GetViewMatrix() * GetWorld();
    gl::glBindVertexArray(m_VAO);

    GLint worldLocation = gl::glGetUniformLocation(m_shader->GetProgram(), "gMVP");
    if(worldLocation == -1)
    {
        std::cout << "Couldn't get uniform loaction" << std::endl;
    }
    gl::glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(mvp));

    gl::glEnableVertexAttribArray(0);
    gl::glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, nullptr);

    gl::glDisableVertexAttribArray(0);
    gl::glDisableVertexAttribArray(1);
}
