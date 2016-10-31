#include "staticmodel.hpp"

#include <memory>

#include <Utilities/utilities.hpp>
#include <Core/glapi.hpp>
#include <Render/Textures/texture.hpp>

using namespace gl;

StaticModel::StaticModel(const std::shared_ptr<GlutDisplay> display, const std::shared_ptr<Model3D> model)
        : m_display(display), m_model(model) {

    if(model == nullptr || display == nullptr) {
        throw new Utilities::Exception(1, "Null param passed to StaticModel constructor");
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

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->GetIndices().size(), mesh->GetIndices().data(), GL_STATIC_DRAW);
        m_indexCount.push_back(mesh->GetIndices().size());
    }

    m_shader = std::make_unique<Shader>();
    std::vector<std::string> files = {"Shaders/staticmodel.vs", "Shaders/staticmodel.fs"};
    std::vector<GLenum> types = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};

    if(!m_shader->InitShader(files, types)) {
        throw new Utilities::Exception(1, "Couldn't intialize shader");
    }
}

StaticModel::~StaticModel() {
    glDeleteVertexArrays(m_VAO.size(), m_VAO.data());
    for(std::vector<GLuint>& vbo : m_VBO) {
        glDeleteBuffers(vbo.size(), vbo.data());
    }
    glDeleteBuffers(m_IBO.size(), m_IBO.data());
}

void StaticModel::Render() {
    m_shader->UseShader();

    GLint worldLocation = gl::glGetUniformLocation(m_shader->GetProgram(), "gMVP");
    if(worldLocation == -1) {
        std::cout << "Couldn't get MVP uniform loaction" << std::endl;
        exit(-1);
    }

    for(uint i = 0; i < m_meshCount; i++) {
        glm::mat4 model;
        if(m_model->GetMeshes()[i]->HasTransformation()) {
            model = GetWorld() * m_model->GetMeshes()[i]->GetTransformation();
        } else {
            model = GetWorld();
        }

        glm::mat4 mvp = m_display->GetProjectionMatrix() * m_display->GetCameraModule()->GetViewMatrix() * model;
        glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(m_VAO[i]);

        GLint samplerLocation = gl::glGetUniformLocation(m_shader->GetProgram(), "gSampler");
        if(samplerLocation == -1) {
            std::cout << "Couldn't get sampler uniform location" << std::endl;
            exit(-1);
        }
        glUniform1i(samplerLocation, 0);

        std::shared_ptr<Texture> texture = m_model->GetTexture(m_model->GetMeshes()[i]->GetMatName());
        if(texture != nullptr) {
            texture->Bind();
        } else {
            std::cout << "Couldn't get material " << m_model->GetMeshes()[i]->GetMatName() << std::endl;
            exit(-1);
        }

        m_sampler.Bind();

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
