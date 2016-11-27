#include "model3d.hpp"

#include <iostream>

// Model3D

Model3D::Model3D() {

}

Model3D::Model3D(const std::vector<std::shared_ptr<Mesh>>& meshes, const std::unordered_map<std::string, std::shared_ptr<Texture>> textures)
    :  m_textures(textures), m_meshes(meshes) {

}

Model3D::~Model3D() {

}

void Model3D::AddMesh(std::shared_ptr<Mesh> mesh) {
    m_meshes.push_back(mesh);
}

void Model3D::SetMeshes(const std::vector<std::shared_ptr<Mesh>>& meshes) {
    m_meshes = meshes;
}

void Model3D::SetTextures(const std::unordered_map<std::string, std::shared_ptr<Texture>> textures) {
    m_textures = textures;
}

void Model3D::ApplyTransformation(const glm::mat4 &transform) {
    for(auto& mesh : m_meshes) {
        mesh->ApplyTransformation(transform);
    }
}

void Model3D::PrintTextures() const {
    for(const auto texture : m_textures) {
        std::cout << texture.first << "->" << (texture.second->IsLoaded() ? "loaded" : "load failed") << ": " << texture.second->GetFileName() << '\n';
    }
}

uint Model3D::GetFaceCount() const {
    uint faceCount = 0;
    for(const auto& mesh : m_meshes) {
        faceCount += mesh->GetIndices().size() / 3;
    }
    return faceCount;
}

std::vector<std::shared_ptr<Model3D::Mesh>> Model3D::GetMeshes() const {
    return m_meshes;
}

std::shared_ptr<Texture> Model3D::GetTexture(const std::string& key) const {
    auto tex = m_textures.find(key);
    if(tex != m_textures.end())
    {
        return tex->second;
    }
    return nullptr;
}

// Model3D::Vertex

Model3D::Vertex::Vertex() {
}

Model3D::Vertex::Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tex)
    : position(pos), normal(norm), texCoord(tex) {

}

// Model3D::Mesh

Model3D::Mesh::Mesh(const std::vector<Vertex> vertices, const std::vector<uint> indices)
: m_matIndex(0), m_matName("null"), m_indices(indices), m_vertices(vertices) {}

Model3D::Mesh::~Mesh() {}

void Model3D::Mesh::ApplyTransformation(const glm::mat4& transform) {
    for(auto& vert : m_vertices) {
        glm::vec4 pos = transform * glm::vec4(vert.position, 1.0f);
        vert.position = glm::vec3(pos.x, pos.y, pos.z);
    }
}

std::vector<Model3D::Vertex> Model3D::Mesh::GetVertices() const {
    return m_vertices;
}

std::vector<uint> Model3D::Mesh::GetIndices() const {
    return m_indices;
}

void Model3D::Mesh::SetMatIndex(const uint matIndex) {
    m_matIndex = matIndex;
}

uint Model3D::Mesh::GetMatIndex() const {
    return m_matIndex;
}

void Model3D::Mesh::SetMatName(const std::string matName) {
    m_matName = matName;
}

std::string Model3D::Mesh::GetMatName() const {
    return m_matName;
}
