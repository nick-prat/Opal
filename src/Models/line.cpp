#include <Models/line.hpp>

#include <vector>
#include <string>
#include <glm/gtc/type_ptr.hpp>

#include <Render/Shader/shader.hpp>
#include <Core/display.hpp>

Line::Line(glm::vec3 tail, glm::vec3 head, glm::vec3 color)
        : m_indexCount(2), m_VAO(0), m_VBO(0), m_tail(tail), m_head(head), m_color(color) {

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    std::vector<glm::vec3> verts = {tail, head};
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * verts.size(), verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glEnableVertexAttribArray(0);

    std::vector<uint> indices = {0, 1};
    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    m_indexCount = indices.size();

    std::vector<std::string> files = {"line_vs.glsl", "line_fs.glsl"};
    std::vector<GLenum> types = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
    m_shader = std::make_unique<Shader>(files, types);
    m_shader->registerUniform("gMVP");
    m_shader->registerUniform("gColor");
}

Line::~Line() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
}

void Line::render(const Display* const display) const {
    glUniformMatrix4fv(m_shader->getUniformLocation("gMVP"), 1, GL_FALSE,
            glm::value_ptr(display->getProjectionMatrix() * display->getCamera()->getViewMatrix()));
    glUniform3fv(m_shader->getUniformLocation("gColor"), 1, (GLfloat*)&m_color);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_LINES, (GLsizei)m_indexCount, GL_UNSIGNED_INT, nullptr);
}
