#include "log.h"

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
    stream << "logs/" << 1900 + parts->tm_year << ":" << 1 + parts->tm_mon << ":" << parts->tm_mday << " - "
            << parts->tm_hour << ":" << parts->tm_min << ":" << parts->tm_sec;
    std::string filename = stream.str();

    m_logFile.open(filename);
}

Log::~Log()
{
    m_logFile.close();
}

void Log::debug(std::string text, short output)
{
    m_log.print(text, output);
}

void Log::error(std::string text, short output)
{
    m_log.print("ERROR: " + text, output);
}

void Log::info(std::string text, short output)
{
    m_log.print("INFO: " + text, output);
}

void Log::print(std::string text, short output)
{
    if(output & OUT_CONS)
    {
        std::cout << text << std::endl;
    }

    if(output & OUT_LOG)
    {
        m_logFile << text << std::endl;
    }
}
