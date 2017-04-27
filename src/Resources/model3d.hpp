#ifndef _MODEL3D_H
#define _MODEL3D_H

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <GL/gl3w.h>

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
        friend class Model3D;
    public:
        Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
        ~Mesh();

        std::vector<Vertex> getVertices() const;
        std::vector<unsigned int> getIndices() const;
        GLuint getVBO() const;
        GLuint getIBO() const;

        void setMatIndex(const unsigned int matIndex);
        unsigned int getMatIndex() const;

        void setMatName(const std::string matName);
        std::string getMatName() const;

        std::size_t getIndexCount() const;

    private:
        unsigned int m_matIndex;
        GLuint m_vbo, m_ibo;
        std::string m_matName;
        std::vector<unsigned int> m_indices;
        std::vector<Vertex> m_vertices;
    };

    Model3D(std::vector<Mesh>&& meshes, const std::unordered_map<std::string, Texture*>& textures);
    Model3D(const Model3D&) = delete;
    Model3D(Model3D&& model);

    Model3D& operator=(Model3D&& model);

    const Texture& getTexture(const std::string& key) const;
    const Mesh& getMesh(unsigned int index) const;
    unsigned int getMeshCount() const;
    unsigned int getFaceCount() const;
    std::vector<GLuint> generateVAOs() const;
    void printTextures() const;

private:
    void generateMeshBuffers();

private:
    std::vector<Mesh> m_meshes;
    std::unordered_map<std::string, Texture*> m_textures;
};

#endif // _MODEL3D_H
