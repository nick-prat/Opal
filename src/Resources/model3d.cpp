#include "model3d.hpp"

#include <iostream>

#include <Resources/texture.hpp>
#include <Utilities/log.hpp>

Model3D::Model3D(std::vector<Mesh>&& meshes, std::unordered_map<std::string, Texture*>&& textures)
: m_meshes(meshes)
, m_textures(textures) {}

const Texture* Model3D::getTexture(const std::string& key) const {
    auto tex = m_textures.find(key);
    if(tex != m_textures.end())
    {
        return tex->second;
    }
    return nullptr;
}

const Model3D::Mesh& Model3D::getMesh(unsigned int index) const {
    return m_meshes[index];
}

unsigned int Model3D::getMeshCount() const {
    return m_meshes.size();
}

unsigned int Model3D::getFaceCount() const {
    unsigned int faceCount = 0;
    for(const auto& mesh : m_meshes) {
        faceCount += mesh.getIndices().size() / 3;
    }
    return faceCount;
}

CRender Model3D::generateRenderComponent() const {
    
}

void Model3D::printTextures() const {
    for(const auto texture : m_textures) {
        Log::getLog() << texture.first << " : " << texture.second->getFileName() << '\n';
    }
}

// Model3D::Vertex

Model3D::Vertex::Vertex() {
}

Model3D::Vertex::Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tex)
        : position(pos), normal(norm), texCoord(tex) {

}

// Model3D::Mesh

Model3D::Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
        : m_matIndex(0), m_matName("null"), m_indices(std::move(indices)), m_vertices(std::move(vertices)) {}

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
