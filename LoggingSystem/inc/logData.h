#pragma once
/***********************************************
* @headerfile logData.h
* @date 16 / 08 / 2024
* @author Roomain
************************************************/
#include <string>
#include <string_view>
#include <chrono>
#include <source_location>

/*@brief enumerate log severity*/
enum class LogSeverity
{
    None = 0,
    Info = 1,
    Warning = 1 << 1,
    Error = 1 << 2,
    Critical = 1 << 3
};

using LogTimePoint = std::chrono::time_point<std::chrono::system_clock>;

/*@brief represents a log*/
struct LogData
{
    LogTimePoint time = std::chrono::system_clock::now();           /*!< log time*/
    LogSeverity severity = LogSeverity::Info;                       /*!< log severity*/
    std::source_location location = std::source_location::current();/*!< log location*/
    std::string message;                                            /*!< log message*/
};
