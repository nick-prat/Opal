#ifndef _ASSIMPMODEL_H
#define _ASSIMPMODEL_H

#include <glm/glm.hpp>
#include <vector>

class AssimpModel
{
public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    class AssimpMesh
    {
    public:
        AssimpMesh(std::vector<Vertex> vertices, std::vector<uint> indices);
        ~AssimpMesh();

        std::vector<Vertex> GetVertices() const;
        std::vector<uint> GetIndices() const;

    private:
        std::vector<uint> m_indices;
        std::vector<Vertex> m_vertices;
    };

    AssimpModel();
    ~AssimpModel();

    void AddMesh(const AssimpMesh& mesh);
    std::vector<AssimpMesh> GetMeshes() const;

private:
    std::vector<AssimpMesh> m_meshes;

};

#endif // _ASSIMPMODEL_H
