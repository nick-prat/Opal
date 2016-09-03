#include "staticmodel.hpp"

#include <Utilities/utilities.hpp>
#include <Core/glapi.hpp>

StaticModel::StaticModel(const std::shared_ptr<GlutDisplay> display, const std::shared_ptr<AssimpModel> model)
    : m_display(display), m_model(model)
{
    m_meshCount = m_model->GetMeshes().size();
    for(uint i = 0; i < m_meshCount; i++)
    {
        AssimpModel::AssimpMesh mesh = m_model->GetMeshes()[i];
        GLuint vbo[3], vao, ibo;

        std::vector<GLuint> vbos;

        gl::glGenVertexArrays(1, &vao);
        gl::glBindVertexArray(vao);
        m_VAO.push_back(vao);

        gl::glGenBuffers(2, vbo);

        gl::glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        gl::glBufferData(GL_ARRAY_BUFFER, sizeof(AssimpModel::Vertex) * mesh.GetVertices().size(), mesh.GetVertices().data(), GL_STATIC_DRAW);
        gl::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpModel::Vertex), 0);
        gl::glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpModel::Vertex), (GLvoid*)sizeof(glm::vec3));

        vbos.push_back(vbo[0]);

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

    m_shader->BindAttribute("Position", 0);
    m_shader->BindAttribute("Normal", 1);

    if(!m_shader->LinkProgram())
    {
        throw new Utilities::Exception(1, "Couldn't link shader");
    }

}

StaticModel::~StaticModel()
{
    gl::glDeleteVertexArrays(m_VAO.size(), m_VAO.data());
    for(std::vector<GLuint> vbo : m_VBO)
    {
        gl::glDeleteBuffers(vbo.size(), vbo.data());
    }
    gl::glDeleteBuffers(m_IBO.size(), m_IBO.data());
}

void StaticModel::Render()
{
    m_shader->UseShader();

    glm::mat4 mvp = m_display->GetProjectionMatrix() * m_display->GetCameraModule()->GetViewMatrix() * GetWorld();

    for(uint i = 0; i < m_meshCount; i++)
    {
        gl::glBindVertexArray(m_VAO[i]);

        GLint worldLocation = gl::glGetUniformLocation(m_shader->GetProgram(), "gMVP");
        if(worldLocation == -1)
        {
            std::cout << "Couldn't get uniform loaction" << std::endl;
        }
        gl::glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(mvp));

        gl::glEnableVertexAttribArray(0);
        gl::glEnableVertexAttribArray(1);

        glDrawElements(GL_TRIANGLES, (GLsizei)m_indexCount.data()[i], GL_UNSIGNED_INT, nullptr);

        gl::glDisableVertexAttribArray(0);
        gl::glDisableVertexAttribArray(1);
    }
}
