#ifndef _ASSIMPMODEL_H
#define _ASSIMPMODEL_H

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <Core/glapi.hpp>
#include <Model/Textures/texture.hpp>

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

        void SetTransformation(const glm::mat4x4& transformation);

        glm::mat4x4 GetTransformation() const;
        std::vector<Vertex> GetVertices() const;
        std::vector<uint> GetIndices() const;

        void SetMatIndex(const uint matIndex);
        uint GetMatIndex() const;

        void SetMatName(const std::string matName);
        std::string GetMatName() const;

    private:
        uint m_matIndex;
        std::string m_matName;
        glm::mat4x4 m_transformation;
        std::vector<uint> m_indices;
        std::vector<Vertex> m_vertices;
    };

    AssimpModel();
    ~AssimpModel();

    void SetMeshes(const std::vector<std::shared_ptr<AssimpMesh>>& meshes);
    void SetTextures(const std::unordered_map<std::string, std::shared_ptr<Texture>>& textures);

    std::shared_ptr<Texture> GetTexture(const std::string& key) const;
    std::vector<std::shared_ptr<AssimpMesh>> GetMeshes() const;

    void PrintTextures();

private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
    std::vector<std::shared_ptr<AssimpMesh>> m_meshes;

};

#endif // _ASSIMPMODEL_H
