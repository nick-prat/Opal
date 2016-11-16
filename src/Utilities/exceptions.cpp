#include "exceptions.hpp"

#include <Utilities/log.hpp>

GenericException::GenericException(const std::string& error) {
    m_error = error;
}

GenericException::~GenericException() {
}

void GenericException::PrintError() const {
    Log::error(m_error, Log::OUT_LOG | Log::OUT_CONS);
}

std::string GenericException::GetError() const {
    return m_error;
}

const char* GenericException::what() const noexcept {
    return m_error.c_str();
}

BadResource::BadResource(const std::string& error, const std::string& filename)
        : GenericException(error), m_filename(filename) {

}

BadResource::~BadResource() {}

std::string BadResource::GetFilename() const {
    return m_filename;
}
