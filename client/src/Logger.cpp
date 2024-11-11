#include <chrono>
#include <iostream>
#include <sstream>
#include "date/date.h"
#include "Logger.hpp"

#ifdef _WIN32
    #define NOGDI  // All GDI defines and routines
    #define NOUSER // All USER defines and routines
extern "C"
{
    #include <windows.h>
}
    // Because raylib uses these names as function parameters
    #undef near
    #undef far
    #undef min
    #undef max
#endif

#ifdef _WIN32
static HANDLE &getHandle()
{
    static HANDLE hConsole(GetStdHandle(STD_OUTPUT_HANDLE));
    return hConsole;
}
#endif

void Logger::print(LogLevel levelT, const std::string &level, const std::string &message)
{
    auto const now = std::chrono::system_clock::now();
    auto it        = getCallbacks().find(levelT);
    std::stringstream s;
    std::string mes;

    {
        using namespace date;
        s << now << " [" << level << "] " << message;
    }
    mes = s.str();

#ifdef _WIN32
    static std::unordered_map<LogLevel, WORD> colors = {
        {LogLevel::Fatal,       static_cast<WORD>(FOREGROUND_RED | FOREGROUND_INTENSITY)                    },
        {LogLevel::Error,       static_cast<WORD>(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)  },
        {LogLevel::Warn,        static_cast<WORD>(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY)},
        {LogLevel::Info,        static_cast<WORD>(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY) },
        {LogLevel::Debug,       static_cast<WORD>(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)},
        {LogLevel::Trace,       static_cast<WORD>(FOREGROUND_RED | FOREGROUND_INTENSITY)                    },
        {LogLevel::MAXLOGLEVEL, static_cast<WORD>(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)      },
    };

    if (getHandle() != INVALID_HANDLE_VALUE) {
        SetConsoleTextAttribute(getHandle(), colors[levelT]);
        std::cerr << mes << std::endl;
        SetConsoleTextAttribute(getHandle(), colors[LogLevel::MAXLOGLEVEL]);
    } else {
        std::cerr << message << std::endl;
    }

#else
    static std::unordered_map<LogLevel, std::string> colors = {
{LogLevel::Fatal,       "\033[31m"},
{LogLevel::Error,       "\033[33m"},
{LogLevel::Warn,        "\033[34m"},
{LogLevel::Info,        "\033[32m"},
{LogLevel::Debug,       "\033[38m"},
{LogLevel::Trace,       "\033[30m"},
{LogLevel::MAXLOGLEVEL, "\033[0m" },
};

std::cerr << colors[levelT] << mes << colors[LogLevel::MAXLOGLEVEL] << std::endl;
#endif

    if (it != getCallbacks().end()) {
        for (auto &it1 : it->second) {
            it1.second(mes);
        }
    }
}
