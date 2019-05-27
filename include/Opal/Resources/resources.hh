#ifndef _RESOURCES_H
#define _RESOURCES_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <string>

#include <Opal/Util/util.hh>

namespace Opal::Resources {

    enum ResType : unsigned char {
        Model3D = 0x01,
        Texture2D,
        Texture3D,
        TextureCube,
        File,
        Shader,
        ShaderVert,
        ShaderGeo,
        ShaderTessCtrl,
        ShaderTessEval,
        ShaderFrag,
        ShaderComp
    };

    struct RFile {
        RFile() = default;
        RFile(const RFile& file) = default;
        RFile(RFile&& file);
        RFile(std::istream& stream);
        RFile(std::vector<char>&& bytes);

        std::vector<char> bytes;
    };

    struct RVertex {
        RVertex() = default;
        RVertex(const RVertex& vert) = default;
        RVertex(RVertex&& vert);
        RVertex(std::istream& stream);
        RVertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tc);

        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    struct RMesh {
        RMesh() = default;
        RMesh(RMesh&& mesh);
        RMesh(std::istream& stream);
        RMesh(std::vector<RVertex>&& vertices, std::vector<unsigned int>&& indices);

        unsigned int matIndex;
        std::string matName;
        std::vector<RVertex> vertices;
        std::vector<unsigned int> indices;
    };

    struct RModel3D {
        RModel3D() = default;
        RModel3D(RModel3D&& m3d);
        RModel3D(std::istream& stream);
        RModel3D(std::vector<RMesh>&& meshes);

        std::string name;
        std::string filename;
        std::vector<RMesh> meshes;
    };

    struct RTexture {
        RTexture() = default;
        RTexture(RTexture&& texture);
        RTexture(std::istream& stream);
        RTexture(std::vector<char>&& bytes, unsigned int width, unsigned int height);

        std::string name;
        std::string filename;
        std::vector<char> bytes;
        unsigned int width, height;
    };

    struct RShader {
        RShader() = default;
        RShader(RShader&& shader);
        RShader(std::istream& stream);
        RShader(std::unordered_map<char, RFile>&& files);

        std::string name;
        std::vector<std::string> uniforms;
        std::unordered_map<char, RFile> files;
    };

    struct RTerrain {
        std::string name;
        std::vector<char> bytes;
        unsigned int width, height;
        glm::vec3 u, v, n;
    };

    struct RObject {
        RObject() = default;
        RObject(std::istream& stream);

        char type;
        std::string resourceName;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    };

    std::ostream& operator<<(std::ostream& stream, const RModel3D& model3d);
    std::ostream& operator<<(std::ostream& stream, const RMesh& mesh);
    std::ostream& operator<<(std::ostream& stream, const RTexture& texture);
    std::ostream& operator<<(std::ostream& stream, const RShader& shader);
    std::ostream& operator<<(std::ostream& stream, const RFile& file);
    std::ostream& operator<<(std::ostream& stream, const RObject& object);

    Util::size_t sizeOf(const RFile& file);
    Util::size_t sizeOf(const RModel3D& model3d);
    Util::size_t sizeOf(const RMesh& mesh);
    Util::size_t sizeOf(const RTexture& texture);
    Util::size_t sizeOf(const RShader& shader);
    Util::size_t sizeOf(const RObject& object);

    RFile loadFile(const std::string& filename);
    RFile loadFile(std::istream& stream);
    std::pair<RModel3D, std::unordered_set<std::string>> loadModel3D(const std::string& filename, const std::string& resourcename);
    RModel3D loadModel3D(std::istream& stream);
    RMesh loadMesh(std::istream& stream);
    RTexture loadTexture(const std::string& filename, const std::string& resourcename);
    RTexture loadTexture(std::istream& stream);
    RShader loadShader(const nlohmann::json& shader);
    RShader loadShader(std::istream& stream);
    RTerrain loadTerrain(nlohmann::json const& terrain);
}

#endif // _RESOURCES_H
