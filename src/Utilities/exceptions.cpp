#include "exceptions.hpp"

#include <Utilities/log.hpp>

GenericException::GenericException(const std::string& error) {
    m_error = error;
}

GenericException::~GenericException() {
}

void GenericException::printError() const {
    Log::error(m_error, Log::OUT_LOG_CONS);
}

std::string GenericException::getError() const {
    return m_error;
}

const char* GenericException::what() const noexcept {
    return m_error.c_str();
}

BadResource::BadResource(const std::string& error, const std::string& resourcename)
        : GenericException(error), m_resourcename(resourcename) {
}

BadResource::~BadResource() {}

void BadResource::printError() const {
    Log::error("[" + m_resourcename + "] " + m_error + '\n', Log::OUT_LOG_CONS);
}

std::string BadResource::getResourceName() const {
    return m_resourcename;
}
