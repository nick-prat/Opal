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
class Texture;

// NOTE What is this responsible for doing?
// NOTE Shoudl shaders be controlled here?

class ResourceHandler {
public:
    ResourceHandler();
    ~ResourceHandler();

    void LoadResources();
    void AddResource(const std::string& name, const IResource* const resource);

    IRenderObject* GenerateModel(const nlohmann::json& obect);
    IRenderObject* GenerateModel(const nlohmann::json& object, const Model3D* const model);
    IRenderObject* LoadLineJSON(const nlohmann::json& object);
    const Texture* LoadTexture(std::string name, bool genMipMaps);
    const Model3D* LoadModel3D(std::string modelname);

    template <typename T = IRenderObject>
    const T* GetResource(const std::string& resource) {
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
    void CopyaiMat(const aiMatrix4x4* from, glm::mat4& to);
    void LoadNode(const aiScene* scene, const aiNode* node, glm::mat4 parentTransform, std::vector<Model3D::Mesh*>& meshes);

private:
    std::unordered_map<std::string, std::unique_ptr<const IResource>> m_resources;
};

#endif // _RESOURCE_HANDLER_H
