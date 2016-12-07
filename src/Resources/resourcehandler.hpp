#ifndef _RESOURCE_HANDLER_H
#define _RESOURCE_HANDLER_H

#include <unordered_map>
#include <memory>

#include <Render/Textures/texture.hpp>
#include <Resources/model3d.hpp>
#include <Resources/resource.hpp>
#include <Render/renderobject.hpp>
#include <Utilities/exceptions.hpp>
#include <json.hpp>

class ResourceHandler {
public:
    ResourceHandler();
    ~ResourceHandler();

    void LoadResources();
    void AddResource(std::string name, Resource* resource);

    IRenderObject* GenerateModel(nlohmann::json obect);
    IRenderObject* GenerateModel(nlohmann::json object, Model3D* model);
    IRenderObject* LoadLineJSON(nlohmann::json object);
    std::shared_ptr<Texture> LoadTexture(std::string filename, bool genMipMaps);
    Model3D* LoadModel3D(std::string modelname);

    template <typename T>
    T* GetResource(std::string resource) {
        auto res = m_resources.find(resource);
        if(res == m_resources.end() || res->second.get() == nullptr) {
            return nullptr;
        }
        auto ret = dynamic_cast<T*>(res->second.get());
        if(ret == nullptr) {
            throw bad_resource("invalid type conversion", resource);
        }
        return ret;
    }

private:
    std::unordered_map<std::string, std::unique_ptr<Resource>> m_resources;
};

#endif // _RESOURCE_HANDLER_H
