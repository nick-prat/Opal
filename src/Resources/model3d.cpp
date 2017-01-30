#include "model3d.hpp"

#include <iostream>

#include <Resources/texture.hpp>
#include <Utilities/log.hpp>

// Model3D

Model3D::Model3D()
        : IResource("model3d") {
}

Model3D::~Model3D() {

}

void Model3D::addMesh(Mesh* mesh) {
    m_meshes.push_back(std::unique_ptr<Mesh>(mesh));
}

void Model3D::addTexture(const std::string& name, const Texture* const texture) {
    m_textures[name] = texture;
}

void Model3D::applyTransformation(const glm::mat4 &transform) {
    for(auto& mesh : m_meshes) {
        mesh->applyTransformation(transform);
    }
}

void Model3D::printTextures() const {
    for(const auto texture : m_textures) {
        Log::getLog() << texture.first << "->" << (texture.second->isLoaded() ? "loaded" : "load failed") << ": " << texture.second->getFileName() << '\n';
    }
}

unsigned int Model3D::getMeshCount() const {
    return m_meshes.size();
}

unsigned int Model3D::getFaceCount() const {
    unsigned int faceCount = 0;
    for(const auto& mesh : m_meshes) {
        faceCount += mesh->getIndices().size() / 3;
    }
    return faceCount;
}

Model3D::Mesh* Model3D::getMesh(unsigned int index) const {
    return m_meshes[index].get();
}

const Texture* Model3D::getTexture(const std::string& key) const {
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

Model3D::Mesh::Mesh(const std::vector<Vertex> vertices, const std::vector<unsigned int> indices)
: m_matIndex(0), m_matName("null"), m_indices(indices), m_vertices(vertices) {}

Model3D::Mesh::~Mesh() {}

void Model3D::Mesh::applyTransformation(const glm::mat4& transform) {
    for(auto& vert : m_vertices) {
        glm::vec4 pos = transform * glm::vec4(vert.position, 1.0f);
        vert.position = glm::vec3(pos.x, pos.y, pos.z);
    }
}

std::vector<Model3D::Vertex> Model3D::Mesh::getVertices() const {
    return m_vertices;
}

std::vector<unsigned int> Model3D::Mesh::getIndices() const {
    return m_indices;
}

void Model3D::Mesh::setMatIndex(const unsigned int matIndex) {
    m_matIndex = matIndex;
}

unsigned int Model3D::Mesh::getMatIndex() const {
    return m_matIndex;
}

void Model3D::Mesh::setMatName(const std::string matName) {
    m_matName = matName;
}

std::string Model3D::Mesh::getMatName() const {
    return m_matName;
}
