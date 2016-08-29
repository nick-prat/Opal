#include "assimpmodel.h"

AssimpModel::AssimpModel()
{

}

AssimpModel::~AssimpModel()
{

}

void AssimpModel::AssimpMesh::SetIndices(std::vector<uint> indices)
{
    m_indices = indices;
}

void AssimpModel::AssimpMesh::SetNormals(std::vector<glm::vec3> normals)
{
    m_normals = normals;
}

void AssimpModel::AssimpMesh::SetVertices(std::vector<glm::vec3> vertices)
{
    m_vertices = vertices;
}

void AssimpModel::AssimpMesh::SetTexCoods(std::vector<glm::vec2> texCoords)
{
        m_texCoords = texCoords;
}

std::vector<glm::vec3> AssimpModel::AssimpMesh::GetNormals() const
{
    return m_normals;
}

std::vector<glm::vec3> AssimpModel::AssimpMesh::GetVertices() const
{
    return m_vertices;
}

std::vector<glm::vec2> AssimpModel::AssimpMesh::GetTexCoords() const
{
    return m_texCoords;
}
