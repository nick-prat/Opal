#include "assimpmodel.h"

AssimpModel::AssimpModel()
{

}

AssimpModel::~AssimpModel()
{

}

void AssimpModel::AddMesh(const AssimpMesh& mesh)
{
    m_meshes.push_back(mesh);
}

std::vector<AssimpModel::AssimpMesh> AssimpModel::GetMeshes() const
{
    return m_meshes;
}

void AssimpModel::AssimpMesh::SetIndices(const std::vector<uint>& indices)
{
    m_indices = indices;
}

void AssimpModel::AssimpMesh::SetNormals(const std::vector<glm::vec3>& normals)
{
    m_normals = normals;
}

void AssimpModel::AssimpMesh::SetVertices(const std::vector<glm::vec3>& vertices)
{
    m_vertices = vertices;
}

void AssimpModel::AssimpMesh::SetTexCoords(const std::vector<std::vector<glm::vec2>>& texCoords)
{
        m_texCoords = texCoords;
}

std::vector<uint> AssimpModel::AssimpMesh::GetIndices() const
{
    return m_indices;
}

std::vector<glm::vec3> AssimpModel::AssimpMesh::GetNormals() const
{
    return m_normals;
}

std::vector<glm::vec3> AssimpModel::AssimpMesh::GetVertices() const
{
    return m_vertices;
}

std::vector<glm::vec2> AssimpModel::AssimpMesh::GetTexCoords(int index) const
{
    if(m_texCoords.size() >= index)
    {
        return m_texCoords[index];
    }
    return {};
}
