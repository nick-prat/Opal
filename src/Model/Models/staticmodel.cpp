#include "staticmodel.h"

#include <Core/glapi.h>

StaticModel::StaticModel(std::shared_ptr<GlutDisplay> display, std::shared_ptr<AssimpModel> model)
    : m_display(display), m_model(model)
{
    for(int i = 0; i < m_model->GetMeshes().size(); i++)
    {
        AssimpModel::AssimpMesh mesh = m_model->GetMeshes()[i];
        GLuint vbo, vao, ibo;

        gl::glGenVertexArrays(1, &vao);
        gl::glBindVertexArray(vao);

        m_VAO.push_back(vao);

        gl::glGenBuffers(1, &vbo);
        gl::glBindBuffer(GL_ARRAY_BUFFER, vbo);
        gl::glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.GetVertices().size(), mesh.GetVertices().data(), GL_STATIC_DRAW);

        gl::glGenBuffers(1, &ibo);
        gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        gl::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.GetIndices().size(), mesh.GetIndices().data(), GL_STATIC_DRAW);
    }


}

StaticModel::~StaticModel()
{

}

void StaticModel::Render()
{

}
