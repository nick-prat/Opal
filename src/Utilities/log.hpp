#ifndef _LOG_H
#define _LOG_H

#include <fstream>
#include <iostream>

// NOTE Is this thread safe?

class Log
{
public:
    enum OutputMode {
        OUT_CONS = 0x0001,
        OUT_LOG = 0x0010,
        OUT_LOG_CONS = 0x0011
    };

    static void debug(std::string text, OutputMode showInConsole = OUT_CONS);
    static void error(std::string text, OutputMode showInConsole = OUT_LOG_CONS);

    static Log& getLog();
    static Log& getErrorLog();

    template<typename T>
    Log& operator<<(T value) {
        if(m_output & OUT_CONS)
        {
            (m_errorLog ? std::cerr : std::cout) << value;
        }

        if(m_output & OUT_LOG)
        {
            m_logFile << value;
        }
        return m_log;
    }
    Log& operator<<(OutputMode value);

private:
    Log();
    ~Log();

    void print(std::string, short output, bool err);

private:
    static Log m_log;
    OutputMode m_output;
    bool m_errorLog;
    std::fstream m_logFile;
};

#endif // _LOG_H
