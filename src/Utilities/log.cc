#include "log.hh"

#include <iostream>

void Log::setLogStream(std::ostream &stream) {
    std::lock_guard<std::mutex> logMutex(m_log->m_logMutex);
    std::lock_guard<std::mutex> errorMutex(m_log->m_errorMutex);
    m_log.reset(new Log{stream, m_log->m_errorStream});
}

void Log::setErrorStream(std::ostream &stream) {
    std::lock_guard<std::mutex> logMutex(m_log->m_logMutex);
    std::lock_guard<std::mutex> errorMutex(m_log->m_errorMutex);
    m_log.reset(new Log{m_log->m_logStream, stream});
}

Log::Log()
: m_logStream(std::cout)
, m_errorStream(std::cerr) {}

Log::Log(std::ostream &logStream, std::ostream &errorStream)
: m_logStream(logStream)
, m_errorStream(errorStream) {}

SyncLogger::SyncLogger(std::ostream &logStream, std::mutex &errorStream)
: ILogger<SyncLogger>(logStream, errorStream) {}

ASyncLogger::ASyncLogger(std::ostream &logStream, std::mutex &errorStream)
: ILogger<ASyncLogger>(logStream, errorStream) {}
