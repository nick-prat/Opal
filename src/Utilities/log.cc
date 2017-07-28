#include "log.hh"

#include <iostream>

void Opal::Log::setLogStream(std::ostream &stream) {
    std::lock_guard<std::mutex> logMutex(m_log->m_logMutex);
    std::lock_guard<std::mutex> errorMutex(m_log->m_errorMutex);
    m_log.reset(new Log{stream, m_log->m_errorStream});
}

void Opal::Log::setErrorStream(std::ostream &stream) {
    std::lock_guard<std::mutex> logMutex(m_log->m_logMutex);
    std::lock_guard<std::mutex> errorMutex(m_log->m_errorMutex);
    m_log.reset(new Log{m_log->m_logStream, stream});
}

Opal::Log::Log()
: m_logStream(std::cout)
, m_errorStream(std::cerr) {}

Opal::Log::Log(std::ostream &logStream, std::ostream &errorStream)
: m_logStream(logStream)
, m_errorStream(errorStream) {}

Opal::SyncLogger::SyncLogger(std::ostream &logStream, std::mutex &errorStream)
: ILogger<SyncLogger>(logStream, errorStream) {}

Opal::ASyncLogger::ASyncLogger(std::ostream &logStream, std::mutex &errorStream)
: ILogger<ASyncLogger>(logStream, errorStream) {}
