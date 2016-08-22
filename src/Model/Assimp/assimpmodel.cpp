#include "assimpmodel.h"

AssimpModel::AssimpModel()
{

}

AssimpModel::~AssimpModel()
{

}

void AssimpModel::SetNormals(std::vector<glm::vec3> normals)
{
    m_normals = normals;
}

void AssimpModel::SetVertices(std::vector<glm::vec3> vertices)
{
    m_vertices = vertices;
}

void AssimpModel::SetTexCoods(std::vector<glm::vec2> texCoords)
{
        m_texCoords = texCoords;
}

std::vector<glm::vec3> AssimpModel::RequestNormals() const
{
    return m_normals;
}

std::vector<glm::vec3> AssimpModel::RequestVertices() const
{
    return m_vertices;
}

std::vector<glm::vec2> AssimpModel::RequestTexCoords() const
{
    return m_texCoords;
}
