#include <Opal/Util/exceptions.hh>
#include <Opal/Util/log.hh>

Opal::GenericException::GenericException(const std::string &error)
: m_error(error) {}

void Opal::GenericException::printError() const {
    Log::getErrorLog<SyncLogger>() << m_error << '\n';
}

std::string Opal::GenericException::getError() const {
    return m_error;
}

const char* Opal::GenericException::what() const noexcept {
    return m_error.c_str();
}

Opal::BadResource::BadResource(const std::string &error, const std::string &resourcename)
: GenericException(error)
, m_resourcename(resourcename) {}

void Opal::BadResource::printError() const {
    Log::getErrorLog<SyncLogger>() << "[" << m_resourcename << "] " << m_error << '\n';
}

std::string Opal::BadResource::getResourceName() const {
    return m_resourcename;
}
