#ifndef _ASSIMPMODEL_H
#define _ASSIMPMODEL_H

#include <glm/glm.hpp>
#include <vector>

class AssimpModel
{
public:
    class AssimpMesh
    {
    public:
        void SetNormals(std::vector<glm::vec3> normals);
        void SetVertices(std::vector<glm::vec3> vertices);
        void SetTexCoods(std::vector<glm::vec2> texCoords);

        std::vector<glm::vec3> RequestNormals() const;
        std::vector<glm::vec3> RequestVertices() const;
        std::vector<glm::vec2> RequestTexCoords() const;

    private:
        std::vector<glm::vec3> m_normals;
        std::vector<glm::vec3> m_vertices;
        std::vector<glm::vec2> m_texCoords;
    };

    AssimpModel();
    ~AssimpModel();

    void AddMesh(const AssimpMesh& mesh);

private:
    std::vector<AssimpMesh> m_meshes;

};

#endif // _ASSIMPMODEL_H
