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
    public:
        Texture();
        ~Texture();

        void SetLoaded(const bool loaded);
        bool IsLoaded() const;

        void SetFileName(const std::string filename);
        std::string GetFileName() const;

    private:
        bool m_loaded;
        std::string m_filename;
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

    void SetMeshes(const std::vector<AssimpMesh>& meshes);
    void SetTextures(const std::unordered_map<std::string, Texture>& textures);

    bool GetTexture(const std::string& key, Texture& texture) const;
    std::vector<AssimpMesh> GetMeshes() const;

    void PrintTextures();

private:
    std::unordered_map<std::string, Texture> m_textures;
    std::vector<AssimpMesh> m_meshes;

};

#endif // _ASSIMPMODEL_H
