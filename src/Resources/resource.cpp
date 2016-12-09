#include <Resources/resource.hpp>

IResource::IResource()
        : m_type("undefined") {

}

IResource::~IResource() {

}

std::string IResource::GetType() {
    return m_type;
}
