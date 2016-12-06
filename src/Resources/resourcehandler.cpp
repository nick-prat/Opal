#include <Resources/resourcehandler.hpp>

ResourceHandler::ResourceHandler() {

}

ResourceHandler::~ResourceHandler() {

}

void ResourceHandler::LoadResources() {

}

void ResourceHandler::AddResource(std::string name, Resource* resource) {
    m_resources[name] = std::unique_ptr<Resource>(resource);
}
