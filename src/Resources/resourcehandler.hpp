#ifndef _RESOURCE_HANDLER_H
#define _RESOURCE_HANDLER_H

#include <unordered_map>
#include <memory>
#include <vector>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <json.hpp>

#include <Utilities/exceptions.hpp>
#include <Resources/model3d.hpp>

class IRenderObject;
class IResource;
class Shader;
class Texture;

// NOTE What is this responsible for doing?

class ResourceHandler {
public:
    ResourceHandler();
    ~ResourceHandler();

    void loadResources(const nlohmann::json& scene);
    void addResource(const std::string& name, const IResource* const resource);

    IRenderObject* generateModel(const nlohmann::json& obect);
    IRenderObject* generateModel(const nlohmann::json& object, const Model3D* const model);
    IRenderObject* generateLine(const nlohmann::json& object);
    Shader* loadShader(const nlohmann::json& object);
    const Texture* loadTexture(const std::string& name, bool genMipMaps);
    const Model3D* loadModel3D(const std::string& modelname);

    Shader* getShader(const std::string& shader);

    template <typename T = IRenderObject>
    const T* getResource(const std::string& resource) {
        auto res = m_resources.find(resource);
        if(res == m_resources.end() || res->second.get() == nullptr) {
            return nullptr;
        }
        auto ret = dynamic_cast<const T*>(res->second.get());
        if(ret == nullptr) {
            throw BadResource("invalid type conversion", resource);
        }
        return ret;
    }

private:
    void copyaiMat(const aiMatrix4x4* from, glm::mat4& to);
    void loadNode(const aiScene* scene, const aiNode* node, glm::mat4 parentTransform, std::vector<Model3D::Mesh*>& meshes);

private:
    std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;
    std::unordered_map<std::string, std::unique_ptr<const IResource>> m_resources;
};

#endif // _RESOURCE_HANDLER_H
