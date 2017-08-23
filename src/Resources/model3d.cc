#include <Opal/Resources/model3d.hh>
#include <Opal/Resources/texture.hh>
#include <Opal/Util/log.hh>
#include <Opal/Util/exceptions.hh>

#include <iostream>

Opal::Model3D::Model3D(const Resources::RModel3D& model3d, std::unordered_map<std::string, Texture*>&& textures)
: m_textures{std::move(textures)} {
    for(auto& rmesh : model3d.meshes) {
        m_meshes.emplace_back();
        auto& mesh = m_meshes.back();

        glGenBuffers(1, &mesh.m_vbo);
        glGenBuffers(1, &mesh.m_ibo);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Resources::RVertex) * rmesh.vertices.size(), rmesh.vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * rmesh.indices.size(), rmesh.indices.data(), GL_STATIC_DRAW);
    }

    for(const auto& mesh : model3d.meshes) {
        for(const auto& vec : mesh.vertices) {
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

Opal::Model3D::Model3D(Model3D&& model)
: m_meshes{model.m_meshes}
, m_textures{std::move(model.m_textures)} {}

Opal::Model3D& Opal::Model3D::operator=(Model3D&& model) {
    m_meshes = std::move(model.m_meshes);
    m_textures = std::move(model.m_textures);
    m_boundingBox = model.m_boundingBox;

    return *this;
}

const Opal::Texture& Opal::Model3D::getTexture(const std::string& key) const {
    if(auto tex{m_textures.find(key)}; tex != m_textures.end()) {
        return *(tex->second);
    } else {
        throw std::invalid_argument{"texture " + key + " not found"};
    }
}

unsigned int Opal::Model3D::getIndexCount(unsigned int index) const {
    return m_meshes[index].m_indexCount;
}

std::string Opal::Model3D::getMatName(unsigned int i) const {
    if(i < m_meshes.size()) {
        return m_meshes[i].m_matName;
    } else {
        throw std::invalid_argument{"Index out of range"};
    }
}

unsigned int Opal::Model3D::getMeshCount() const {
    return m_meshes.size();
}

unsigned int Opal::Model3D::getFaceCount() const {
    auto faceCount{0u};
    for(const auto& mesh : m_meshes) {
        faceCount += mesh.m_indexCount / 3;
    }
    return faceCount;
}

// TODO Maybe this isn't the best way to do this
std::vector<GLuint> Opal::Model3D::generateVAOs() const {
    std::vector<GLuint> vaos;
    vaos.reserve(m_meshes.size());
    glGenVertexArrays(m_meshes.size(), vaos.data());

    for(auto i = 0u; i < m_meshes.size(); i++) {
        glBindVertexArray(vaos[i]);

        glBindBuffer(GL_ARRAY_BUFFER, m_meshes[i].m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshes[i].m_ibo);

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
    for(const auto& texture : m_textures) {
        Log::getLog<SyncLogger>() << texture.first << " : " << texture.second->getFileName() << '\n';
    }
}
