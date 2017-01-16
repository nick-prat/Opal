#ifndef _MODEL3D_H
#define _MODEL3D_H

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <Resources/resource.hpp>

class Texture;

// NOTE Are there model types that aren't 3D, if so implement them
class Model3D : public IResource
{
public:
    // NOTE Does a vertex struct need any functions?
    struct Vertex
    {
        Vertex();
        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord);
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    // NOTE Should a mesh be a POD structure?
    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex> vertices, const std::vector<unsigned int> indices);
        ~Mesh();

        void applyTransformation(const glm::mat4& transform);

        std::vector<Vertex> getVertices() const;
        std::vector<unsigned int> getIndices() const;

        void setMatIndex(const unsigned int matIndex);
        unsigned int getMatIndex() const;

        void setMatName(const std::string matName);
        std::string getMatName() const;

    private:
        unsigned int m_matIndex;
        std::string m_matName;
        glm::mat4x4 m_transformation;
        std::vector<unsigned int> m_indices;
        std::vector<Vertex> m_vertices;
    };

    Model3D();
    ~Model3D();

    // NOTE Add meshes and texture is not possible in resource handler, so should I have these functions?
    void addMesh(Mesh* mesh);
    void addTexture(const std::string& name, const Texture* const texture);

    // NOTE Is this function useful still?
    void applyTransformation(const glm::mat4& transform);

    const Texture* getTexture(const std::string& key) const;
    Mesh* getMesh(unsigned int index) const;
    unsigned int getMeshCount() const;
    unsigned int getFaceCount() const;

    void printTextures() const;

private:
    std::unordered_map<std::string, const Texture*> m_textures;

    // NOTE It might be useful to have these as obect's not pointers for easier copying
    std::vector<std::unique_ptr<Mesh>> m_meshes;

};

#endif // _MODEL3D_H
