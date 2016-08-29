#include "staticmodel.h"

#include <Core/glapi.h>

StaticModel::StaticModel(std::shared_ptr<GlutDisplay> display, std::shared_ptr<AssimpModel> model)
    : m_display(display), m_model(model)
{
    gl::glGenVertexArrays(1, &m_VAO);
    gl::glBindVertexArray(m_VAO);

    gl::glGenBuffers(1, m_VBO);
    gl::glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
    gl::glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_model->GetVertices().size(), m_model->GetVertices().data(), GL_STATIC_DRAW);


}

StaticModel::~StaticModel()
{

}

void StaticModel::Render()
{

}
