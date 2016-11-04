#include <Models/line.hpp>

#include <vector>
#include <string>

#include <Core/glapi.hpp>
#include <Utilities/utilities.hpp>

using namespace gl;
using Utilities::Exception;

Line::Line(std::shared_ptr<GlutDisplay> display, glm::vec3 tail, glm::vec3 head, glm::vec3 color)
        : m_indexCount(2), m_VAO(0), m_VBO(0), m_tail(tail), m_head(head), m_color(color), m_shader(nullptr), m_display(display) {

    if(display == nullptr) {
        throw Exception("Null parameter passed to Line constructor");
    }

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    std::vector<glm::vec3> verts = {tail, head};
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * verts.size(), verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    std::vector<uint> indices = {0, 1};
    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    m_indexCount = indices.size();

    std::vector<std::string> files = {"Shaders/line.vs", "Shaders/line.fs"};
    std::vector<GLenum> types = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
    m_shader = std::make_unique<Shader>(files, types);
}

Line::~Line() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
}

void Line::Render() {
    m_shader->UseShader();

    GLint worldLocation = glGetUniformLocation(m_shader->GetProgram(), "gMVP");
    if(worldLocation == -1) {
        std::cout << "Couldn't get MVP uniform loaction" << std::endl;
        exit(-1);
    }

    glm::mat4 mvp = m_display->GetProjectionMatrix() * m_display->GetCameraModule()->GetViewMatrix() * GetWorld();
    glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(mvp));

    GLint colorLocation = glGetUniformLocation(m_shader->GetProgram(), "gColor");
    if(colorLocation == -1) {
        std::cout << "Couldn't get line color uniform location" << std::endl;
        exit(-1);
    }
    glUniform3fv(colorLocation, 1, (GLfloat*)&m_color);

    glBindVertexArray(m_VAO);
    glEnableVertexAttribArray(0);
    glDrawElements(GL_LINES, (GLsizei)m_indexCount, GL_UNSIGNED_INT, nullptr);
    glDisableVertexAttribArray(0);
}
