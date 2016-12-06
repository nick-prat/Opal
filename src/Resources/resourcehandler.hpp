#ifndef _RESOURCE_HANDLER_H
#define _RESOURCE_HANDLER_H

#include <unordered_map>
#include <memory>

#include <Resources/resource.hpp>

class ResourceHandler {
public:
    ResourceHandler();
    ~ResourceHandler();

    void LoadResources();
    void AddResource(std::string name, Resource* resource);

    template <typename T>
    T* GetResource(std::string resource) {
        auto res = m_resources.find(resource);
        if(res == m_resources.end()) {
            return nullptr;
        }
        return dynamic_cast<T*>(res->second.get());
    }

private:
    std::unordered_map<std::string, std::unique_ptr<Resource>> m_resources;
};

#endif // _RESOURCE_HANDLER_H
