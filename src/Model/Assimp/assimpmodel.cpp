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

// AssimpModel::Texture

AssimpModel::Texture::Texture()
{}

AssimpModel::Texture::~Texture()
{
    if(m_loaded)
    {
        glDeleteTextures(1, &m_glTexture);
        m_glTexture = 0;
    }
}

void AssimpModel::Texture::SetLoaded(const bool loaded)
{
    m_loaded = loaded;
}

bool AssimpModel::Texture::IsLoaded() const
{
    return m_loaded;
}

void AssimpModel::Texture::SetFileName(const std::string filename)
{
    m_filename = filename;
}

std::string AssimpModel::Texture::GetFileName() const
{
    return m_filename;
}

void AssimpModel::Texture::SetTexture(const GLuint glTexture)
{
    m_glTexture = glTexture;
}

// AssimpModel::AssimpMesh

AssimpModel::AssimpMesh::AssimpMesh(std::vector<Vertex> vertices, std::vector<uint> indices)
: m_indices(indices), m_vertices(vertices)
{}

AssimpModel::AssimpMesh::~AssimpMesh()
{}

std::vector<AssimpModel::Vertex> AssimpModel::AssimpMesh::GetVertices() const
{
    return m_vertices;
}

std::vector<uint> AssimpModel::AssimpMesh::GetIndices() const
{
    return m_indices;
}
