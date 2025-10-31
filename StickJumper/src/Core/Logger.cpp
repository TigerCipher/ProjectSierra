#include "Logger.h"

#include <chrono>
#include <cstdio>

namespace stick::logger
{

namespace
{
std::string CurrentTimestamp()
{
    using namespace std::chrono;
    const auto  now = system_clock::now();
    const auto  ms  = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    std::time_t tt  = system_clock::to_time_t(now);

    std::tm tm;
    int     err = 0;
#ifdef _WIN32
    err = localtime_s(&tm, &tt);
#else
    localtime_r(&tt, &tm);
#endif

    char buf[64];
    err = std::snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d.%03d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                        tm.tm_hour, tm.tm_min, tm.tm_sec, static_cast<int>(ms.count()));
    return { buf };
}
} // namespace

void detail::Output(const LogLevel::Level level, std::string_view message, std::string_view fileName, u32 lineNumber)
{
    std::string currentTime = CurrentTimestamp();
    std::string str;
    switch (level)
    {
    case LogLevel::Debug: str = std::format("[{}][{}:{}][ DEBUG ]: {}\n", currentTime, fileName, lineNumber, message); break;
    case LogLevel::Info: str = std::format("[{}][{}:{}][ INFO ]: {}\n", currentTime, fileName, lineNumber, message); break;
    case LogLevel::Warn: str = std::format("[{}][{}:{}][ WARNING ]: {}\n", currentTime, fileName, lineNumber, message); break;
    case LogLevel::Error: str = std::format("[{}][{}:{}][ ERROR ]: {}\n", currentTime, fileName, lineNumber, message); break;
    case LogLevel::Fatal: str = std::format("[{}][{}:{}][ FATAL ]: {}\n", currentTime, fileName, lineNumber, message); break;
    }

    if (level == LogLevel::Warn || level == LogLevel::Error || level == LogLevel::Fatal)
    {
        i32 err = std::fprintf(stderr, "%s", str.c_str());
        err     = std::fflush(stderr);
    } else
    {
        std::printf("%s", str.c_str());
        i32 err = std::fflush(stdout);
    }
}

} // namespace stick::logger