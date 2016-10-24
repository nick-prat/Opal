#include "assimpmodel.hpp"

#include <iostream>

// AssimpModel

AssimpModel::AssimpModel()
{

}

AssimpModel::~AssimpModel()
{

}

void AssimpModel::SetMeshes(const std::vector<std::shared_ptr<AssimpMesh>>& meshes)
{
    m_meshes = meshes;
}

void AssimpModel::SetTextures(const std::unordered_map<std::string, std::shared_ptr<Texture>>& textures)
{
    m_textures = textures;
}

void AssimpModel::PrintTextures()
{
    for(const auto texture : m_textures)
    {
        std::cout << texture.first << "->" << (texture.second->IsLoaded() ? "loaded: " : "load failed: ") << ": " << texture.second->GetFileName() << std::endl;
    }
}

std::vector<std::shared_ptr<AssimpModel::AssimpMesh>> AssimpModel::GetMeshes() const
{
    return m_meshes;
}

std::shared_ptr<Texture> AssimpModel::GetTexture(const std::string& key) const
{
    auto tex = m_textures.find(key);
    if(tex != m_textures.end())
    {
        return tex->second;
    }
    return nullptr;
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
