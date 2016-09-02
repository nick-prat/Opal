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

AssimpModel::AssimpMesh::AssimpMesh(std::vector<Vertex> vertices, std::vector<uint> indices)
: m_indices(indices), m_vertices(vertices) 
{

}

AssimpModel::AssimpMesh::~AssimpMesh()
{

}

std::vector<AssimpModel::Vertex> AssimpModel::AssimpMesh::GetVertices() const
{
    return m_vertices;
}

std::vector<uint> AssimpModel::AssimpMesh::GetIndices() const
{
    return m_indices;
}
