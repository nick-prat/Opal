#ifndef _ASSIMPMODEL_H
#define _ASSIMPMODEL_H

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
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

    class Texture
    {

    };

    class AssimpMesh
    {
    public:
        AssimpMesh(std::vector<Vertex> vertices, std::vector<uint> indices);
        ~AssimpMesh();

        void SetTransformation(const glm::mat4x4& transformation);

        glm::mat4x4 GetTransformation() const;
        std::vector<Vertex> GetVertices() const;
        std::vector<uint> GetIndices() const;

    private:
        glm::mat4x4 m_transformation;
        std::vector<uint> m_indices;
        std::vector<Vertex> m_vertices;
    };

    AssimpModel();
    ~AssimpModel();

    void SetMeshes(const std::vector<AssimpMesh>& meshes);
    void SetTextures(const std::unordered_map<std::string, Texture>& textures);

    bool GetTexture(const std::string& key, Texture& texture) const;
    std::vector<AssimpMesh> GetMeshes() const;

private:
    std::unordered_map<std::string, Texture> m_textures;
    std::vector<AssimpMesh> m_meshes;

};

#endif // _ASSIMPMODEL_H
