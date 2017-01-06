#ifndef _MODEL3D_H
#define _MODEL3D_H

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <Resources/resource.hpp>

class Texture;

class Model3D : public IResource
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

        void applyTransformation(const glm::mat4& transform);

        std::vector<Vertex> getVertices() const;
        std::vector<uint> getIndices() const;

        void setMatIndex(const uint matIndex);
        uint getMatIndex() const;

        void setMatName(const std::string matName);
        std::string getMatName() const;

    private:
        uint m_matIndex;
        std::string m_matName;
        glm::mat4x4 m_transformation;
        std::vector<uint> m_indices;
        std::vector<Vertex> m_vertices;
    };

    Model3D();
    ~Model3D();

    void addMesh(Mesh* mesh);
    void addTexture(const std::string& name, const Texture* const texture);

    void applyTransformation(const glm::mat4& transform);

    const Texture* getTexture(const std::string& key) const;
    Mesh* getMesh(uint index) const;
    uint getMeshCount() const;
    uint getFaceCount() const;

    void printTextures() const;

private:
    std::unordered_map<std::string, const Texture*> m_textures;
    std::vector<std::unique_ptr<Mesh>> m_meshes;

};

#endif // _MODEL3D_H
