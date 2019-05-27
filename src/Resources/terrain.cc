#include <Opal/Resources/terrain.hh>

Opal::TerrainPatch::TerrainPatch(Resources::RTerrain const& terrain) {
    m_patch.reserve(terrain.width * terrain.height);
    m_indices.reserve((terrain.width - 1) * (terrain.height - 1) * 6);

    for(int i = 0; i < terrain.width; i++) {
        for(int j = 0; j < terrain.height; j++) {
            const float h = terrain.bytes[i * terrain.width + j] / 255.0f;
            m_patch.emplace_back(
                (terrain.u * static_cast<float>(i)) + (terrain.v * static_cast<float>(j)) + (h * terrain.n),
                terrain.n,
                glm::vec2{static_cast<float>(i) / (terrain.width - 1), static_cast<float>(j) / (terrain.height - 1)}
            );

            if(i < terrain.width - 1 && j < terrain.height - 1) {
                m_indices.push_back(i * terrain.width + j);
                m_indices.push_back(i * terrain.width + (j + 1));
                m_indices.push_back((i + 1) * terrain.width + j);

                m_indices.push_back((i + 1) * terrain.width + j);
                m_indices.push_back(i * terrain.width + (j + 1));
                m_indices.push_back((i + 1) * terrain.width + (j + 1));
            }
        }
    }


    glCreateBuffers(2, m_buffers.data());
    glNamedBufferStorage(m_buffers[0], sizeof(Vertex) * m_patch.size(), m_patch.data(), GL_STATIC_DRAW);
    glNamedBufferStorage(m_buffers[1], sizeof(unsigned int) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

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
    glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec3));
    glVertexArrayAttribBinding(m_vao, 2, 0);
}
