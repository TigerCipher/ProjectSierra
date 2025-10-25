#include "Logger.h"

#include <cstdio>

namespace stick::logger
{

void detail::Output(const LogLevel::Level level, std::string_view message, std::string_view fileName, u32 lineNumber)
{
    std::string str;
    switch (level)
    {
    case LogLevel::Debug: str = std::format("[{}:{}][ DEBUG ]: {}\n", fileName, lineNumber, message); break;
    case LogLevel::Info: str = std::format("[{}:{}][ INFO ]: {}\n", fileName, lineNumber, message); break;
    case LogLevel::Warn: str = std::format("[{}:{}][ WARNING ]: {}\n", fileName, lineNumber, message); break;
    case LogLevel::Error: str = std::format("[{}:{}][ ERROR ]: {}\n", fileName, lineNumber, message); break;
    case LogLevel::Fatal: str = std::format("[{}:{}][ FATAL ]: {}\n", fileName, lineNumber, message); break;
    }

    if (level == LogLevel::Warn || level == LogLevel::Error || level == LogLevel::Fatal)
    {
        i32 err = std::fprintf(stderr, "%s", str.c_str());
        err = std::fflush(stderr);
    } else
    {
        std::printf("%s", str.c_str());
        i32 err = std::fflush(stdout);
    }
}

} // namespace stick::logger