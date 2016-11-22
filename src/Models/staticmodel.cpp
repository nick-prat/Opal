#include "staticmodel.hpp"

#include <memory>
#include <iostream>

#include <Utilities/utilities.hpp>
#include <Utilities/exceptions.hpp>
#include <GL/gl3w.h>
#include <Render/Textures/texture.hpp>

StaticModel::StaticModel(const std::shared_ptr<Model3D> model)
        : m_model(model) {

    if(model == nullptr) {
        throw generic_exception("Null param passed to StaticModel constructor");
    }

    m_meshCount = m_model->GetMeshes().size();

    for(uint i = 0; i < m_meshCount; i++) {
        std::shared_ptr<Model3D::Mesh> mesh = m_model->GetMeshes()[i];
        GLuint vbo, vao, ibo;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        m_VAO.push_back(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Model3D::Vertex) * mesh->GetVertices().size(), mesh->GetVertices().data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model3D::Vertex), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model3D::Vertex), (GLvoid*)sizeof(glm::vec3));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Model3D::Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
        m_VBO.push_back(vbo);

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->GetIndices().size(), mesh->GetIndices().data(), GL_STATIC_DRAW);
        m_indexCount.push_back(mesh->GetIndices().size());
        m_IBO.push_back(ibo);
    }

    std::vector<std::string> files = {"staticmodel_vs.glsl", "staticmodel_fs.glsl"};
    std::vector<GLenum> types = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
    m_shader = std::make_unique<Shader>(files, types);
}

StaticModel::~StaticModel() {
    glDeleteVertexArrays(m_VAO.size(), m_VAO.data());
    glDeleteBuffers(m_VBO.size(), m_VBO.data());
    glDeleteBuffers(m_IBO.size(), m_IBO.data());
}

void StaticModel::Render(const std::shared_ptr<Display> display) {
    m_shader->UseShader();

    GLint worldLocation = glGetUniformLocation(m_shader->GetProgram(), "gMVP");
    if(worldLocation == -1) {
        std::cout << "Couldn't get MVP uniform loaction" << std::endl;
        exit(-1);
    }

    GLint samplerLocation = glGetUniformLocation(m_shader->GetProgram(), "gSampler");
    if(samplerLocation == -1) {
        std::cout << "Couldn't get sampler uniform location" << std::endl;
        exit(-1);
    }
    glUniform1i(samplerLocation, 0);

    m_sampler.Bind();

    for(uint i = 0; i < m_meshCount; i++) {
        glm::mat4 model;
        if(m_model->GetMeshes()[i]->HasTransformation()) {
            model = GetWorld() * m_model->GetMeshes()[i]->GetTransformation();
        } else {
            model = GetWorld();
        }

        glm::mat4 mvp = display->GetProjectionMatrix() * display->GetCamera()->GetViewMatrix() * model;
        glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(m_VAO[i]);

        std::shared_ptr<Texture> texture = m_model->GetTexture(m_model->GetMeshes()[i]->GetMatName());
        if(texture != nullptr) {
            texture->Bind();
        } else {
            std::cout << "Couldn't get material " << m_model->GetMeshes()[i]->GetMatName() << std::endl;
            exit(-1);
        }

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glDrawElements(GL_TRIANGLES, (GLsizei)m_indexCount.data()[i], GL_UNSIGNED_INT, nullptr);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }

    Utilities::PrintGLErrors();
}

std::shared_ptr<Model3D> StaticModel::GetModel() {
    return m_model;
}
