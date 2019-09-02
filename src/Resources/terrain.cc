#include <Opal/Resources/terrain.hh>
#include <random>

#include <iostream>

Opal::TerrainPatch::TerrainPatch(Resources::RTerrain const& terrain) {
    std::vector<Vertex> patch;
    std::vector<unsigned int> indices;

    int sampleCount = terrain.width / terrain.sampleRate + 1;

    float skip = terrain.size.x / (sampleCount - 1);

    auto const& bytes = terrain.bytes.data();

    for(int i = 0; i < sampleCount; ++i) {
        auto const scanLine = bytes + (i * (sampleCount - 1) * terrain.sampleRate);

        for(int j = 0; j < sampleCount; ++j) {
            float const height = scanLine[j * terrain.sampleRate] / 256.0f * terrain.size.y;

            patch.emplace_back(
                Vertex{
                    glm::vec3{skip * i, height, skip * j},
                    glm::vec3{0.0f},
                    glm::vec2{static_cast<float>(i) / (sampleCount - 1), static_cast<float>(j) / (sampleCount - 1)}
                }
            );

            if(i < sampleCount - 1 && j < sampleCount - 1) {
                indices.push_back(i * sampleCount + j);
                indices.push_back(i * sampleCount + (j + 1));
                indices.push_back((i + 1) * sampleCount + (j + 1));

                indices.push_back(i * sampleCount + j);
                indices.push_back((i + 1) * sampleCount + (j + 1));
                indices.push_back((i + 1) * sampleCount + j);
            }

        }
    }

    m_indexCount = indices.size();

    glCreateBuffers(2, m_buffers.data());
    glNamedBufferStorage(m_buffers[0], sizeof(Vertex) * patch.size(), patch.data(), GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(m_buffers[1], sizeof(unsigned int) * indices.size(), indices.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &m_vao);
    glVertexArrayVertexBuffer(m_vao, 0, m_buffers[0], 0, sizeof(Vertex));
    glVertexArrayElementBuffer(m_vao, m_buffers[1]);

    glEnableVertexArrayAttrib(m_vao, 0);
    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(m_vao, 0, 0);

    glEnableVertexArrayAttrib(m_vao, 1);
    glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3));
    glVertexArrayAttribBinding(m_vao, 1, 0);

    glEnableVertexArrayAttrib(m_vao, 2);
    glVertexArrayAttribFormat(m_vao, 2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec3));
    glVertexArrayAttribBinding(m_vao, 2, 0);
}

Opal::TerrainPatch::~TerrainPatch() {
    glDeleteBuffers(2, m_buffers.data());
}

GLuint Opal::TerrainPatch::getVAO() {
    return m_vao;
}

unsigned int Opal::TerrainPatch::getIndexCount() const {
    return m_indexCount;
}