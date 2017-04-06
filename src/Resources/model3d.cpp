#include "model3d.hpp"

#include <iostream>

#include <Resources/texture.hpp>
#include <Utilities/log.hpp>

Model3D::Model3D(const std::vector<Mesh>& meshes, const std::unordered_map<std::string, Texture*>& textures)
: m_meshes(generateMeshBuffers(std::move(meshes)))
, m_textures(textures) {}

Model3D::Model3D(Model3D&& model)
: m_meshes(std::move(model.m_meshes))
, m_textures(std::move(model.m_textures)) {}

const Texture* Model3D::getTexture(const std::string& key) const {
    auto tex = m_textures.find(key);
    if(tex != m_textures.end()) {
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
    std::vector<GLuint> vaos;

    for(const auto& mesh : m_meshes) {
        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_ibo);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model3D::Vertex), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model3D::Vertex), (GLvoid*)sizeof(glm::vec3));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Model3D::Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        vaos.push_back(vao);
    }

    return CRender(std::move(vaos));
}

void Model3D::printTextures() const {
    for(const auto texture : m_textures) {
        Log::getLog() << texture.first << " : " << texture.second->getFileName() << '\n';
    }
}

std::vector<Model3D::Mesh> Model3D::generateMeshBuffers(const std::vector<Model3D::Mesh>& meshes) {
    std::vector<Model3D::Mesh> newMeshes;

    for(auto mesh : meshes) {
        glGenBuffers(1, &mesh.m_vbo);
        glGenBuffers(1, &mesh.m_ibo);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Model3D::Vertex) * mesh.getVertices().size(), mesh.getVertices().data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.m_ibo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * mesh.getIndices().size(), mesh.getIndices().data(), GL_STATIC_DRAW);

        newMeshes.push_back(mesh);
    }

    return newMeshes;
}

// Model3D::Vertex

Model3D::Vertex::Vertex() {
}

Model3D::Vertex::Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tex)
: position(pos), normal(norm), texCoord(tex) {}

// Model3D::Mesh

Model3D::Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
: m_matIndex(0), m_matName("null"), m_indices(std::move(indices)), m_vertices(std::move(vertices)) {}

Model3D::Mesh::~Mesh() {
    if(glIsBuffer(m_vbo)) {
        glDeleteBuffers(1, &m_vbo);
    }
    if(glIsBuffer(m_ibo)) {
        glDeleteBuffers(1, &m_ibo);
    }
}

std::vector<Model3D::Vertex> Model3D::Mesh::getVertices() const {
    return m_vertices;
}

std::vector<unsigned int> Model3D::Mesh::getIndices() const {
    return m_indices;
}

GLuint Model3D::Mesh::getVBO() const {
    return m_vbo;
}

GLuint Model3D::Mesh::getIBO() const {
    return m_ibo;
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
