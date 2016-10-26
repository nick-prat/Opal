#include "model3d.hpp"

#include <iostream>

// Model3D

Model3D::Model3D()
{

}

Model3D::~Model3D()
{

}

void Model3D::SetMeshes(const std::vector<std::shared_ptr<Mesh>>& meshes)
{
    m_meshes = meshes;
}

void Model3D::SetTextures(const std::unordered_map<std::string, std::shared_ptr<Texture>>& textures)
{
    m_textures = textures;
}

void Model3D::PrintTextures()
{
    for(const auto texture : m_textures)
    {
        std::cout << texture.first << "->" << (texture.second->IsLoaded() ? "loaded: " : "load failed: ") << ": " << texture.second->GetFileName() << std::endl;
    }
}

std::vector<std::shared_ptr<Model3D::Mesh>> Model3D::GetMeshes() const
{
    return m_meshes;
}

std::shared_ptr<Texture> Model3D::GetTexture(const std::string& key) const
{
    auto tex = m_textures.find(key);
    if(tex != m_textures.end())
    {
        return tex->second;
    }
    return nullptr;
}

// Model3D::Mesh

Model3D::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint> indices)
: m_matIndex(0), m_matName("null"), m_indices(indices), m_vertices(vertices)
{}

Model3D::Mesh::~Mesh()
{}

void Model3D::Mesh::SetTransformation(const glm::mat4x4& transformation)
{
    m_transformation = transformation;
}

glm::mat4x4 Model3D::Mesh::GetTransformation() const
{
    return m_transformation;
}

std::vector<Model3D::Vertex> Model3D::Mesh::GetVertices() const
{
    return m_vertices;
}

std::vector<uint> Model3D::Mesh::GetIndices() const
{
    return m_indices;
}

void Model3D::Mesh::SetMatIndex(const uint matIndex)
{
    m_matIndex = matIndex;
}

uint Model3D::Mesh::GetMatIndex() const
{
    return m_matIndex;
}

void Model3D::Mesh::SetMatName(const std::string matName)
{
    m_matName = matName;
}

std::string Model3D::Mesh::GetMatName() const
{
    return m_matName;
}
