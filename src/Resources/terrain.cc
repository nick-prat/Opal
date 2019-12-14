#include <cmath>
#include <random>
#include <iostream>

#include "Opal/GL/gl3w.h"
#include "Opal/Resources/resources.hh"

#include <Opal/Resources/terrain.hh>

Opal::TerrainPatch::TerrainPatch(Resources::RTerrain const& terrain)
: m_texture{terrain.texture} {
    std::vector<Vertex> patch;
    std::vector<unsigned int> indices;

    // TODO Implement sampleRate
    // auto& const sampleRate = terrain.sampleRate;
    auto const sampleRate = 1;

    auto const sampleCount = terrain.width / sampleRate;
    auto const skip = terrain.size.x / (terrain.width - 1);
    auto const bytes = terrain.bytes.data();

    for(int i = 0; i < terrain.width; i += sampleRate) {
        for(int j = 0; j < terrain.height; j += sampleRate) {
            float const height = bytes[i * terrain.width + j] / 255.0f * terrain.size.y;

            patch.emplace_back(
                Vertex{
                    glm::vec3{skip * i, height, skip * j},
                    glm::vec3{0.0f},
                    glm::vec2{static_cast<float>(i) / (terrain.width - 1), static_cast<float>(j) / (terrain.width - 1)}
                }
            );

            if(i < terrain.width - sampleRate && j < terrain.width - sampleRate) {
                int const indexi = i / sampleRate;
                int const indexj = j / sampleRate;

                // Top Left Face
                indices.push_back(indexi * sampleCount + indexj);
                indices.push_back(indexi * sampleCount + (indexj + 1));
                indices.push_back((indexi + 1) * sampleCount + (indexj + 1));

                // Bottom Right Face
                indices.push_back(indexi * sampleCount + indexj);
                indices.push_back((indexi + 1) * sampleCount + (indexj + 1));
                indices.push_back((indexi + 1) * sampleCount + indexj);
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