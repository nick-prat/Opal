#include <Opal/Resources/model3d.hh>
#include <Opal/Resources/texture.hh>
#include <Opal/Util/log.hh>
#include <Opal/Util/exceptions.hh>

#include <iostream>

Opal::Model3D::Model3D(Resources::RModel3D &&model3d, std::unordered_map<std::string, Texture*> &&textures)
: RModel3D{std::move(model3d)}
, m_textures{std::move(textures)} {
    generateMeshBuffers();
    generateBoundingBox();
}

Opal::Model3D::Model3D(Model3D &&model)
: RModel3D{std::move(model)}
, m_textures{std::move(model.m_textures)} {}

Opal::Model3D &Opal::Model3D::operator=(Model3D &&model) {
    meshes = std::move(model.meshes);
    m_textures = std::move(model.m_textures);
    m_boundingBox = model.m_boundingBox;

    return *this;
}

const Opal::Texture &Opal::Model3D::getTexture(const std::string &key) const {
    if(auto tex{m_textures.find(key)}; tex != m_textures.end()) {
        return *(tex->second);
    } else {
        throw std::invalid_argument{"texture " + key + " not found"};
    }
}

unsigned int Opal::Model3D::getIndexCount(unsigned int index) const {
    return meshes[index].indices.size();
}

std::string Opal::Model3D::getMatName(unsigned int i) const {
    if(i < meshes.size()) {
        return meshes[i].matName;
    } else {
        throw std::invalid_argument{"Index out of range"};
    }
}

unsigned int Opal::Model3D::getMeshCount() const {
    return meshes.size();
}

unsigned int Opal::Model3D::getFaceCount() const {
    auto faceCount{0u};
    for(const auto &mesh : meshes) {
        faceCount += mesh.indices.size() / 3;
    }
    return faceCount;
}

std::vector<GLuint> Opal::Model3D::generateVAOs() const {
    std::vector<GLuint> vaos;
    vaos.reserve(meshes.size());
    glGenVertexArrays(meshes.size(), vaos.data());

    for(auto i = 0u; i < meshes.size(); i++) {
        glBindVertexArray(vaos[i]);

        glBindBuffer(GL_ARRAY_BUFFER, m_meshVBOs[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshIBOs[i]);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Resources::RVertex), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Resources::RVertex), (GLvoid*)sizeof(glm::vec3));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Resources::RVertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }

    return vaos;
}

void Opal::Model3D::printTextures() const {
    for(const auto &texture : m_textures) {
        Log::getLog<SyncLogger>() << texture.first << " : " << texture.second->getFileName() << '\n';
    }
}

void Opal::Model3D::generateMeshBuffers() {
    for(auto &mesh : meshes) {
        GLuint vbo, ibo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Resources::RVertex) * mesh.vertices.size(), mesh.vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.indices.size(), mesh.indices.data(), GL_STATIC_DRAW);

        m_meshVBOs.push_back(vbo);
        m_meshIBOs.push_back(ibo);
    }
}

void Opal::Model3D::generateBoundingBox() {
    for(const auto &mesh : meshes) {
        for(const auto &vec : mesh.vertices) {
            if(vec.position[0] > m_boundingBox[1].x) {
                m_boundingBox[1].x = vec.position[0];
            }
            if(vec.position[0] < m_boundingBox[0].x) {
                m_boundingBox[0].x = vec.position[0];
            }
            if(vec.position[1] > m_boundingBox[1].y) {
                m_boundingBox[1].y = vec.position[1];
            }
            if(vec.position[1] < m_boundingBox[0].y) {
                m_boundingBox[0].y = vec.position[1];
            }
            if(vec.position[2] > m_boundingBox[1].z) {
                m_boundingBox[1].z = vec.position[2];
            }
            if(vec.position[2] < m_boundingBox[0].z) {
                m_boundingBox[0].z = vec.position[2];
            }
        }
    }
}
