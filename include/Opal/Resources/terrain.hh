#ifndef __TERRAIN_H
#define __TERRAIN_H

#include <vector>
#include <glm/glm.hpp>

#include <Opal/Core/gl.hh>
#include <Opal/Resources/resources.hh>

namespace Opal {

    class TerrainPatch {
    private:
        struct Vertex {
            glm::vec3 pos;
            glm::vec3 normal;
            glm::vec2 tex;
        };

    public:
        // TerrainPatch(char const * const bytes, int const width, int const height, float const& u, float const& v, float const& n);
        TerrainPatch(Resources::RTerrain const& texture);
        ~TerrainPatch();

        GLuint getVAO();
        unsigned int getIndexCount() const;

    private:
        std::array<GLuint, 2> m_buffers;
        unsigned int m_indexCount;
        GLuint m_vao;
    };

}

#endif // __TERRIAN_H