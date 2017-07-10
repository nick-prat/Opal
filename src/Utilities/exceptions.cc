#include "exceptions.hh"

#include <Utilities/log.hh>

GenericException::GenericException(const std::string &error)
: m_error(error) {}

void GenericException::printError() const {
    Log::getErrorLog<SyncLogger>() << m_error << '\n';
}

std::string GenericException::getError() const {
    return m_error;
}

const char* GenericException::what() const noexcept {
    return m_error.c_str();
}

BadResource::BadResource(const std::string &error, const std::string &resourcename)
: GenericException(error)
, m_resourcename(resourcename) {}

void BadResource::printError() const {
    Log::getErrorLog<SyncLogger>() << "[" << m_resourcename << "] " << m_error << '\n';
}

std::string BadResource::getResourceName() const {
    return m_resourcename;
}
