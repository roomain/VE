/***********************************************
* @headerfile stringUtils.h
* @date 06 / 08 / 2024
* @author Roomain
************************************************/
#pragma once
#include <cctype>
#include <string>
#include <ranges>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <iomanip>

inline std::string to_string(const std::chrono::system_clock::time_point& a_timepoint)
{
    auto time = a_timepoint.time_since_epoch();
    const int hours = static_cast<int>(std::chrono::duration_cast<std::chrono::hours>(time).count());
    time -= std::chrono::hours(hours);
    const int minutes = static_cast<int>(std::chrono::duration_cast<std::chrono::minutes>(time).count());
    time -= std::chrono::minutes(minutes);
    const int seconds = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(time).count());
    time -= std::chrono::seconds(seconds);
    const int milliseconds = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(time).count());
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(2) << (hours % 24) << ":"
        << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << seconds << "."
        << std::setfill('0') << std::setw(3) << milliseconds;
    return stream.str();
}

inline void trimLeft(std::string& a_toTrim)
{
    a_toTrim.erase(a_toTrim.begin(), std::find_if(a_toTrim.begin(), a_toTrim.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

inline void trimRight(std::string& a_toTrim)
{
    a_toTrim.erase(std::find_if(a_toTrim.rbegin(), a_toTrim.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), a_toTrim.end());
}

[[nodiscard]] inline std::string trim(const std::string& a_toTrim)
{
    std::string temp = a_toTrim;
    trimLeft(temp);
    trimRight(temp);
    return temp;
}

[[nodiscard]] inline std::vector<const char*> vStringToChar(const std::vector<std::string>& a_vString)
{
    std::vector<const char*> vChar;
	std::transform(a_vString.cbegin(), a_vString.cend(), std::back_inserter(vChar),
			[](const auto& a_text) {return a_text.c_str(); });
    return vChar;
}


[[nodiscard]] inline  bool char_equals(const char a_first, const char a_second)
{
    return std::tolower(a_first) == std::tolower(a_second);
}

[[nodiscard]] constexpr  bool iCompare(const std::string& a_first, const std::string& a_second)
{
    return std::equal(a_first.cbegin(), a_first.cend(), 
    a_second.cbegin(), a_second.cend(), char_equals);
}

constexpr void split(const std::string& a_entry, const char a_separator, std::vector<std::string>& a_splitted)
{
    std::string temp = a_entry;
    while(!temp.empty())
    {
        if(auto iter = std::ranges::find(temp, a_separator); iter != temp.cend())
        {
            const size_t index = iter - temp.cbegin();
            a_splitted.emplace_back(temp.substr(0, iter - temp.cbegin()));
            temp = temp.substr(index + 1, temp.size() - index - 1);
        }
        else
        {
            a_splitted.emplace_back(temp);
            temp = "";
        }
    }
}