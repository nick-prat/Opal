#include "staticmodel.hpp"

#include <memory>
#include <GL/gl3w.h>
#include <glm/gtc/type_ptr.hpp>

#include <Utilities/log.hpp>
#include <Utilities/exceptions.hpp>
#include <Render/Sampler/sampler.hpp>
#include <Core/display.hpp>
#include <Core/camera.hpp>
#include <Resources/shader.hpp>
#include <Resources/texture.hpp>
#include <Resources/model3d.hpp>

// NOTE Can i make this process faster?
// NOTE Why should i have a seperate class for dynamic and static models
StaticModel::StaticModel(const Model3D* const model, const glm::mat4& world)
        : m_model(model), m_world(world), m_sampler(std::make_unique<Sampler>()) {

    if(model == nullptr) {
        throw GenericException("Null param passed to StaticModel constructor");
    }

    m_meshCount = m_model->getMeshCount();

    for(uint i = 0; i < m_meshCount; i++) {
        auto mesh = m_model->getMesh(i);
        GLuint vbo, vao, ibo;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        m_VAO.push_back(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Model3D::Vertex) * mesh->getVertices().size(), mesh->getVertices().data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model3D::Vertex), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model3D::Vertex), (GLvoid*)sizeof(glm::vec3));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Model3D::Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
        m_VBO.push_back(vbo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->getIndices().size(), mesh->getIndices().data(), GL_STATIC_DRAW);
        m_indexCount.push_back(mesh->getIndices().size());
        m_IBO.push_back(ibo);
    }

    // NOTE This creates a new shader for every statid model (and consequently every dynamic model)
    std::vector<std::string> files = {"staticmodel_vs.glsl", "staticmodel_fs.glsl"};
    std::vector<GLenum> types = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
    m_shader = std::make_unique<Shader>(files, types);
    m_shader->registerUniform("gSampler");
    m_shader->registerUniform("gMVP");
}

StaticModel::~StaticModel() {
    glDeleteVertexArrays(m_VAO.size(), m_VAO.data());
    glDeleteBuffers(m_VBO.size(), m_VBO.data());
    glDeleteBuffers(m_IBO.size(), m_IBO.data());
}

glm::mat4 StaticModel::generateMVP(const Display* const display) const {
    return display->getProjectionMatrix() * display->getCamera()->getViewMatrix() * m_world;
}

const Model3D* StaticModel::getModel() const {
    return m_model;
}

void StaticModel::render(const Display* const display) const {
    glUniform1i(m_shader->getUniformLocation("gSampler"), 0);
    glUniformMatrix4fv(m_shader->getUniformLocation("gMVP"), 1, GL_FALSE, glm::value_ptr(generateMVP(display)));

    m_sampler->bind();

    for(uint i = 0; i < m_meshCount; i++) {
        glBindVertexArray(m_VAO[i]);

        auto texture = m_model->getTexture(m_model->getMesh(i)->getMatName());
        if(texture != nullptr) {
            texture->bind();
        } else {
            Log::getErrorLog() << "Couldn't get material " << m_model->getMesh(i)->getMatName() << '\n';
            exit(-1);
        }

        glDrawElements(GL_TRIANGLES, (GLsizei)m_indexCount.data()[i], GL_UNSIGNED_INT, nullptr);
    }
}
