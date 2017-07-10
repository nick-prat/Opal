#ifndef _RESOURCE_HANDLER_H
#define _RESOURCE_HANDLER_H

#include <unordered_map>
#include <memory>
#include <vector>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <json.hpp>

#include <Utilities/exceptions.hh>
#include <Render/shader.hh>
#include <Resources/model3d.hh>
#include <Resources/texture.hh>

// TODO Might be a good idea to store any resource in ogl buffers instead

class IRenderObject;
class IResource;
class Texture;

class ResourceHandler {
public:
    void loadResources(const nlohmann::json &scene);
    void addResource(const std::string &name, const IResource* const resource);

    const std::unordered_map<std::string, Shader> &getShaders() const;
    Shader &getShader(const std::string &shader);
    void loadShader(const nlohmann::json &object);

    Texture &getTexture(const std::string &name);
    void loadTexture(const std::string &resourcename, const std::string &filename, bool genMipMaps);

    Model3D &getModel3D(const std::string &name);
    void loadModel3D(const std::string &resourcename, const std::string &filename);

private:
    void copyaiMat(const aiMatrix4x4* from, glm::mat4 &to);
    void loadNode(const aiScene* scene, const aiNode* node, const glm::mat4 &parentTransform, std::vector<Model3D::Mesh> &meshes);

private:
    std::unordered_map<std::string, Shader> m_shaders;
    std::unordered_map<std::string, Texture> m_textures;
    std::unordered_map<std::string, Model3D> m_model3Ds;
};

#endif // _RESOURCE_HANDLER_H
