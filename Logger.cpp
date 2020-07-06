#include <iostream>
#include <cstdlib>
#include <chrono>

#include "Logger.h"

//#define LOG_NO_COLOR

void Logger::fatal(const std::string &message, int exitCode)
{
    using namespace std::chrono;

    #ifndef LOG_NO_COLOR
        std::cerr << "\033[30;41m" << "[Fatal]" << "\033[0;0m: " << message << '\n';
    #else
        std::cerr << "[" <<
                     duration_cast<milliseconds>(
                         system_clock::now().time_since_epoch()).count() <<
                     "][Fatal]: " << message << '\n';
    #endif
    std::cerr.flush();
    
    std::exit(exitCode);
}

void Logger::error(const std::string &message)
{
    using namespace std::chrono;

    #ifndef LOG_NO_COLOR
        std::cerr << "\033[31m" << "[Error]" << "\033[0;0m: " << message << '\n';
    #else
        std::cerr << "[" <<
                     duration_cast<milliseconds>(
                         system_clock::now().time_since_epoch()).count() <<
                     "][Error]: " << message << '\n';
    #endif
    
    std::cerr.flush();
}

void Logger::warning(const std::string &message)
{
    using namespace std::chrono;

    #ifndef LOG_NO_COLOR
        std::cerr << "\033[33m" << "[Warning]" << "\033[0;0m: " << message << '\n';
    #else
        std::cerr << "[" <<
                     duration_cast<milliseconds>(
                         system_clock::now().time_since_epoch()).count() <<
                     "][Warning]: " << message << '\n';
    #endif

    std::cerr.flush();
}

void Logger::info(const std::string &message)
{
    using namespace std::chrono;

    #ifndef LOG_NO_COLOR
        std::cout << "\033[32m" << "[Info]" << "\033[0;0m: " << message << '\n';
    #else
        std::cout << "[" <<
                     duration_cast<milliseconds>(
                         system_clock::now().time_since_epoch()).count() <<
                     "][Info]: " << message << '\n';
    #endif
    
    std::cout.flush();
}
