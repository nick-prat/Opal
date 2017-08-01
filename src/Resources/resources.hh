#ifndef _RESOURCES_H
#define _RESOURCES_H

#include <fstream>
#include <vector>
#include <array>
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Opal::Resources {

    struct RVertex {
        RVertex() = default;
        RVertex(const RVertex& vert) = default;
        RVertex(RVertex &&vert);
        RVertex(std::istream &stream);
        RVertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tc);

        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    struct RMesh {
        RMesh() = default;
        RMesh(RMesh &&mesh);
        RMesh(std::istream &stream);
        RMesh(std::vector<RVertex> &&vertices, std::vector<unsigned int> &&indices);

        unsigned int matIndex;
        std::string matName;
        std::vector<RVertex> vertices;
        std::vector<unsigned int> indices;
    };

    struct RModel3D {
        RModel3D() = default;
        RModel3D(RModel3D &&m3d);
        RModel3D(std::istream &stream);
        RModel3D(std::vector<RMesh> &&meshes);

        std::string name;
        std::vector<RMesh> meshes;
    };

    struct RTexture {
        RTexture() = default;
        RTexture(RTexture &&texture);
        RTexture(std::istream &stream);
        RTexture(std::vector<unsigned char> &&bytes, unsigned int width, unsigned int height);

        std::string name;
        std::vector<unsigned char> bytes;
        unsigned int width, height;
    };

    struct RShader {
        RShader() = default;
        RShader(RShader &&shader);
        RShader(std::istream &stream);
        RShader(std::unordered_map<char, std::vector<unsigned char>> files);

        std::string name;
        std::unordered_map<char, std::vector<unsigned char>> files;
    };

    struct Object {
        Object() = default;
        Object(std::istream &stream);

        std::size_t size() const;
        void writeToStream(std::ostream &stream);

        char type;
        std::string resourceName;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    };

    std::ostream &operator<<(std::ostream &stream, const RModel3D &model3d);
    std::ostream &operator<<(std::ostream &stream, const RMesh &mesh);
    std::ostream &operator<<(std::ostream &stream, const RVertex &vertex);
    std::ostream &operator<<(std::ostream &stream, const RTexture &texture);
    std::ostream &operator<<(std::ostream &stream, const RShader &shader);

    std::size_t size(const RModel3D &model3d);
    std::size_t size(const RMesh &mesh);
    std::size_t size(const RVertex &vertex);
    std::size_t size(const RTexture &texture);
    std::size_t size(const RShader &shader);

    std::pair<RModel3D, std::unordered_map<std::string, RTexture>> loadModel3D(const std::string &filename);
    RModel3D loadModel3D(std::istream &stream);
    RTexture loadTexture(const std::string &filename);
    RTexture loadTexture(std::istream &stream);
    RShader loadShader(const std::string &filename);
    RShader loadShader(std::istream &stream);
}

#endif // _RESOURCES_H
