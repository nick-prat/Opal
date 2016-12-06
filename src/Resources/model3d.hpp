#ifndef _3DMODEL_H
#define _3DMODEL_H

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <GL/gl3w.h>
#include <Resources/resource.hpp>
#include <Render/Textures/texture.hpp>

class Model3D : public Resource
{
public:
    struct Vertex
    {
        Vertex();
        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord);
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex> vertices, const std::vector<uint> indices);
        ~Mesh();

        void ApplyTransformation(const glm::mat4& transform);

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

    Model3D();
    Model3D(const std::vector<std::shared_ptr<Mesh>>& meshes, const std::unordered_map<std::string, std::shared_ptr<Texture>> textures);
    ~Model3D();

    void AddMesh(std::shared_ptr<Mesh> mesh);
    void SetMeshes(const std::vector<std::shared_ptr<Mesh>>& meshes);
    void SetTextures(const std::unordered_map<std::string, std::shared_ptr<Texture>> textures);

    void ApplyTransformation(const glm::mat4& transform);

    std::shared_ptr<Texture> GetTexture(const std::string& key) const;
    std::vector<std::shared_ptr<Mesh>> GetMeshes() const;
    uint GetFaceCount() const;

    void PrintTextures() const;

private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
    std::vector<std::shared_ptr<Mesh>> m_meshes;

};

#endif // _3DMODEL_H
