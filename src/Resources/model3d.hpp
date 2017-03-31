#ifndef _MODEL3D_H
#define _MODEL3D_H

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <ECS/components.hpp>

class Texture;

// NOTE Are there model types that aren't 3D, if so implement them
class Model3D {
public:
    // NOTE Does a vertex struct need any functions?
    struct Vertex {
        Vertex();
        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord);
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    class Mesh {
    public:
        Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

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

    Model3D(std::vector<Mesh>&& meshes, std::unordered_map<std::string, Texture*>&& textures);

    const Texture* getTexture(const std::string& key) const;
    const Mesh& getMesh(unsigned int index) const;
    unsigned int getMeshCount() const;
    unsigned int getFaceCount() const;
    CRender generateRenderComponent() const;

    void printTextures() const;

private:
    const std::vector<Mesh> m_meshes;
    const std::unordered_map<std::string, Texture*> m_textures;
};

#endif // _MODEL3D_H
