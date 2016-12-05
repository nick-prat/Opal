#include "log.hpp"

#include <chrono>
#include <ctime>
#include <sstream>

Log Log::m_log;

Log::Log()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    struct tm *parts = std::localtime(&now_c);

    std::stringstream stream;
    stream << "./logs/" << 1900 + parts->tm_year << ":" << 1 + parts->tm_mon << ":" << parts->tm_mday << " - "
            << parts->tm_hour << ":" << parts->tm_min << ":" << parts->tm_sec;
    std::string filename = stream.str();

    m_logFile.open(filename, std::fstream::out);
    m_logFile << std::flush;
    if(!m_logFile.is_open()) {
        Log::error("Couldn't open log file", Log::OUT_CONS);
    }
}

Log::~Log()
{
    m_logFile.close();
}

void Log::debug(std::string text, short output)
{
    m_log.print(text, output, false);
}

void Log::error(std::string text, short output)
{
    m_log.print("ERROR: " + text, output, true);
}

void Log::info(std::string text, short output)
{
    m_log.print("INFO: " + text, output, false);
}

void Log::print(std::string text, short output, bool err)
{
    if(output & OUT_CONS)
    {
        ((err) ? std::cerr : std::cout) << text << '\n';
    }

    if(output & OUT_LOG)
    {
        m_logFile << text << '\n';
    }
}
