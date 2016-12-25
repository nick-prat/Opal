#include <Resources/resource.hpp>

IResource::IResource(const std::string& type)
        : m_type(type) {

}

IResource::~IResource() {

}

std::string IResource::getType() {
    return m_type;
}
