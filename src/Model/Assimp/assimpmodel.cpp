#include "assimpmodel.hpp"

#include <iostream>

// AssimpModel

AssimpModel::AssimpModel()
{

}

AssimpModel::~AssimpModel()
{

}

void AssimpModel::SetMeshes(const std::vector<AssimpMesh> &meshes)
{
    m_meshes = meshes;
}

void AssimpModel::SetTextures(const std::unordered_map<std::string, Texture> &textures)
{
    m_textures = textures;
}

void AssimpModel::PrintTextures()
{
    for(auto it = m_textures.begin(); it != m_textures.end(); it++)
    {
        std::cout << (it->second.IsLoaded() ? "loaded: " : "load failed: ") << ": " << it->second.GetFileName() << std::endl;
    }
}

std::vector<AssimpModel::AssimpMesh> AssimpModel::GetMeshes() const
{
    return m_meshes;
}

bool AssimpModel::GetTexture(const std::string& key, Texture& texture) const
{
    auto tex = m_textures.find(key);
    if(tex != m_textures.end())
    {
        texture = tex->second;
        return true;
    }
    return false;
}

// AssimpModel::AssimpMesh

AssimpModel::AssimpMesh::AssimpMesh(std::vector<Vertex> vertices, std::vector<uint> indices)
: m_matIndex(0), m_matName("null"), m_indices(indices), m_vertices(vertices)
{}

AssimpModel::AssimpMesh::~AssimpMesh()
{}

void AssimpModel::AssimpMesh::SetTransformation(const glm::mat4x4& transformation)
{
    m_transformation = transformation;
}

glm::mat4x4 AssimpModel::AssimpMesh::GetTransformation() const
{
    return m_transformation;
}

std::vector<AssimpModel::Vertex> AssimpModel::AssimpMesh::GetVertices() const
{
    return m_vertices;
}

std::vector<uint> AssimpModel::AssimpMesh::GetIndices() const
{
    return m_indices;
}

void AssimpModel::AssimpMesh::SetMatIndex(const uint matIndex)
{
    m_matIndex = matIndex;
}

uint AssimpModel::AssimpMesh::GetMatIndex() const
{
    return m_matIndex;
}

void AssimpModel::AssimpMesh::SetMatName(const std::string matName)
{
    m_matName = matName;
}

std::string AssimpModel::AssimpMesh::GetMatName() const
{
    return m_matName;
}
