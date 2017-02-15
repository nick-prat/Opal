#include "staticmodel.hpp"

#include <memory>
#include <GL/gl3w.h>
#include <glm/gtc/type_ptr.hpp>

#include <Utilities/log.hpp>
#include <Utilities/exceptions.hpp>
#include <Render/Sampler/sampler.hpp>
#include <Core/display.hpp>
#include <Core/camera.hpp>
#include <Render/renderchain.hpp>
#include <Resources/texture.hpp>
#include <Resources/model3d.hpp>

// NOTE Can i make this process faster?
// NOTE Why should i have a seperate class for dynamic and static models
StaticModel::StaticModel(const Model3D& model, const glm::mat4& world)
        : m_model(model), m_world(world), m_meshCount(model.getMeshCount()) {

    // TODO Can you store multiple VBOs in a single VAO?
    for(unsigned int i = 0; i < m_meshCount; i++) {
        auto mesh = m_model.getMesh(i);
        GLuint vbo, vao, ibo;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        m_VAO.push_back(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Model3D::Vertex) * mesh.getVertices().size(), mesh.getVertices().data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model3D::Vertex), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model3D::Vertex), (GLvoid*)sizeof(glm::vec3));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Model3D::Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
        m_VBO.push_back(vbo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.getIndices().size(), mesh.getIndices().data(), GL_STATIC_DRAW);
        m_indexCount.push_back(mesh.getIndices().size());
        m_IBO.push_back(ibo);
    }
}

StaticModel::StaticModel(StaticModel&& model) : m_model(model.m_model), m_world(model.m_world), m_meshCount(model.m_meshCount) {
    m_sampler = std::move(model.m_sampler);
    m_indexCount = std::move(model.m_indexCount);
    m_VAO = std::move(model.m_VAO);
    m_VBO = std::move(model.m_VBO);
    m_IBO = std::move(model.m_IBO);
}

StaticModel::~StaticModel() {
    glDeleteVertexArrays(m_VAO.size(), m_VAO.data());
    glDeleteBuffers(m_VBO.size(), m_VBO.data());
    glDeleteBuffers(m_IBO.size(), m_IBO.data());
}

glm::mat4 StaticModel::generateMVP(const Display& display) const {
    return display.getProjectionMatrix() * display.getCamera()->getViewMatrix() * m_world;
}

const Model3D& StaticModel::getModel() const {
    return m_model;
}

void StaticModel::render(const Shader& shader, const Display& display) const {
    glUniform1i(shader.getUniformLocation("gSampler"), 0);
    glUniformMatrix4fv(shader.getUniformLocation("gMVP"), 1, GL_FALSE, glm::value_ptr(generateMVP(display)));

    m_sampler.bind();

    for(unsigned int i = 0; i < m_meshCount; i++) {
        glBindVertexArray(m_VAO[i]);

        auto texture = m_model.getTexture(m_model.getMesh(i).getMatName());
        if(texture != nullptr) {
            texture->bind();
        } else {
            Log::getErrorLog() << "Couldn't get material " << m_model.getMesh(i).getMatName() << '\n';
            exit(-1);
        }

        glDrawElements(GL_TRIANGLES, (GLsizei)m_indexCount.data()[i], GL_UNSIGNED_INT, nullptr);
    }
}
