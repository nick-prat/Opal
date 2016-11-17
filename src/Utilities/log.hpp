#ifndef _LOG_H
#define _LOG_H

#include <fstream>
#include <iostream>

class Log
{
public:
    const static short OUT_CONS = 0x0001;
    const static short OUT_LOG = 0x0010;
    const static short OUT_LOG_CONS = 0x0011;

    static void debug(std::string text, short showInConsole = OUT_CONS);
    static void error(std::string text, short showInConsole = OUT_LOG_CONS);
    static void info(std::string text, short showInConsole = OUT_LOG_CONS);

private:
    Log();
    ~Log();

    void print(std::string, short output);

private:
    static Log m_log;
    std::fstream m_logFile;
};

#endif // _LOG_H
