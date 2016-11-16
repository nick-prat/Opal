#include "exceptions.hpp"

#include <Utilities/log.hpp>

generic_exception::generic_exception(const std::string& error) {
    m_error = error;
}

generic_exception::~generic_exception() {
}

void generic_exception::PrintError() const {
    Log::error(m_error, Log::OUT_LOG | Log::OUT_CONS);
}

std::string generic_exception::GetError() const {
    return m_error;
}

const char* generic_exception::what() const noexcept {
    return m_error.c_str();
}

bad_resource::bad_resource(const std::string& error, const std::string& resourcename)
        : generic_exception(error), m_resourcename(resourcename) {

}

bad_resource::~bad_resource() {}

std::string bad_resource::GetResourceName() const {
    return m_resourcename;
}
