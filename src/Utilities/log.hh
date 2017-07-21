#ifndef _LOG_H
#define _LOG_H

#include <memory>
#include <mutex>
#include <iostream>
#include <queue>
#include <thread>

template<typename logger_t>
class ILogger {
public:
    ILogger(std::ostream &stream, std::mutex &mutex)
    : m_stream(stream)
    , m_mutex(mutex) {}

    template<typename T>
    logger_t &operator<<(const T &value) {
        return static_cast<logger_t*>(this)->operator<<(std::forward<T>(value));
    }

protected:
    std::ostream &m_stream;
    std::mutex &m_mutex;
};

// TODO Implement this
class ASyncLogger : public ILogger<ASyncLogger> {
public:
    ASyncLogger(std::ostream &stream, std::mutex &mutex);

    template<typename T>
    ASyncLogger &operator<<(const T &value) {
        std::thread([&stream = m_stream, &mutex = m_mutex, &value] {
            std::lock_guard<std::mutex> mut(mutex);
            stream << value;
        }).detach();
        return *this;
    }

};

class SyncLogger : public ILogger<SyncLogger> {
public:
    SyncLogger(std::ostream &stream, std::mutex &mutex);

    template<typename T>
    SyncLogger &operator<<(const T &value) {
        std::lock_guard<std::mutex> mut(m_mutex);
        m_stream << value;
        return *this;
    }
};

class Log
{
    friend std::unique_ptr<Log> std::make_unique<Log>();
    friend std::unique_ptr<Log> std::make_unique<Log>(std::ostream &logStream, std::ostream &errorStream);
public:

    static void setLogStream(std::ostream &stream);
    static void setErrorStream(std::ostream &stream);

    template<typename logger_t>
    static logger_t getLog() {
        static_assert(std::is_base_of<ILogger<logger_t>, logger_t>::value, "Specified logger type doesn't inherit ILogger");
        return logger_t(m_log->m_logStream, m_log->m_logMutex);
    }

    template<typename logger_t>
    static logger_t getErrorLog() {
        static_assert(std::is_base_of<ILogger<logger_t>, logger_t>::value, "Specified logger type doesn't inherit ILogger");
        return  logger_t(m_log->m_logStream, m_log->m_logMutex);
    }

private:
    Log();
    Log(std::ostream &logStream, std::ostream &errorStream);

    void print(std::string, short output, bool err);

private:
    inline static std::unique_ptr<Log> m_log = std::make_unique<Log>();
    std::mutex m_logMutex, m_errorMutex;
    std::ostream &m_logStream, &m_errorStream;
};

#endif // _LOG_H
