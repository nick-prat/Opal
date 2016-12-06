#include <Resources/resource.hpp>

Resource::Resource()
        : m_type("undefined") {

}

Resource::~Resource() {

}

std::string Resource::GetType() {
    return m_type;
}
