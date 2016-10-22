#include "staticmodel.hpp"

#include <memory>

#include <Utilities/utilities.hpp>
#include <Core/glapi.hpp>
#include <Model/Textures/sampler.hpp>
#include <Model/Textures/texture.hpp>

StaticModel::StaticModel(const std::shared_ptr<GlutDisplay> display, const std::shared_ptr<AssimpModel> model)
    : m_display(display), m_model(model)
{
    if(model == nullptr || display == nullptr)
    {
        throw new Utilities::Exception(1, "Null param passed to StaticModel constructor");
    }

    m_meshCount = m_model->GetMeshes().size();
    for(uint i = 0; i < m_meshCount; i++)
    {
        AssimpModel::AssimpMesh mesh = m_model->GetMeshes()[i];
        GLuint vbo, vao, ibo;

        std::vector<GLuint> vbos;

        gl::glGenVertexArrays(1, &vao);
        gl::glBindVertexArray(vao);
        m_VAO.push_back(vao);

        gl::glGenBuffers(1, &vbo);
        gl::glBindBuffer(GL_ARRAY_BUFFER, vbo);
        gl::glBufferData(GL_ARRAY_BUFFER, sizeof(AssimpModel::Vertex) * mesh.GetVertices().size(), mesh.GetVertices().data(), GL_STATIC_DRAW);
        gl::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpModel::Vertex), 0);
        gl::glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpModel::Vertex), (GLvoid*)sizeof(glm::vec3));
        gl::glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AssimpModel::Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

        vbos.push_back(vbo);

        gl::glGenBuffers(1, &ibo);
        gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        gl::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.GetIndices().size(), mesh.GetIndices().data(), GL_STATIC_DRAW);
        m_indexCount.push_back(mesh.GetIndices().size());
    }

    m_shader = std::make_unique<Shader>();
    std::vector<std::string> files = {"Shaders/staticmodel.vs", "Shaders/staticmodel.fs"};
    std::vector<GLenum> types = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};

    if(!m_shader->InitShader(files, types))
    {
        throw new Utilities::Exception(1, "Couldn't intialize shader");
    }
}

StaticModel::~StaticModel()
{
    gl::glDeleteVertexArrays(m_VAO.size(), m_VAO.data());
    for(std::vector<GLuint>& vbo : m_VBO)
    {
        gl::glDeleteBuffers(vbo.size(), vbo.data());
    }
    gl::glDeleteBuffers(m_IBO.size(), m_IBO.data());
}

void StaticModel::Render()
{
    m_shader->UseShader();

    for(uint i = 0; i < m_meshCount; i++)
    {
        glm::mat4 mvp = m_display->GetProjectionMatrix() * m_display->GetCameraModule()->GetViewMatrix() * (GetWorld() * m_model->GetMeshes()[i].GetTransformation());

        gl::glBindVertexArray(m_VAO[i]);

        GLint worldLocation = gl::glGetUniformLocation(m_shader->GetProgram(), "gMVP");
        if(worldLocation == -1)
        {
            std::cout << "Couldn't get MVP uniform loaction" << std::endl;
            exit(-1);
        }
        gl::glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(mvp));

        GLint samplerLocation = gl::glGetUniformLocation(m_shader->GetProgram(), "gSampler");
        if(samplerLocation == -1)
        {
            std::cout << "Couldn't get sampler uniform location" << std::endl;
            exit(-1);
        }
        gl::glUniform1i(samplerLocation, 0);

        Texture texture;
        if(m_model->GetTexture(m_model->GetMeshes()[i].GetMatName(), texture))
        {
            texture.Bind();
        }
        else
        {
            std::cout << "Couldn't get material " << m_model->GetMeshes()[i].GetMatName() << std::endl;
            exit(-1);
        }

        m_sampler.Bind();

        gl::glEnableVertexAttribArray(0);
        gl::glEnableVertexAttribArray(1);
        gl::glEnableVertexAttribArray(2);

        glDrawElements(GL_TRIANGLES, (GLsizei)m_indexCount.data()[i], GL_UNSIGNED_INT, nullptr);

        gl::glDisableVertexAttribArray(0);
        gl::glDisableVertexAttribArray(1);
        gl::glDisableVertexAttribArray(2);
    }

    exit(0);
}

std::shared_ptr<AssimpModel> StaticModel::GetModel()
{
    return m_model;
}
