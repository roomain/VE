#pragma once
#include <functional>
#include <string>
#include <source_location>

using LogFun = std::function<void(const std::string&, const std::source_location&)>;

class LocalLogger
{
private:
    static LogFun m_info;
    static LogFun m_warning;
    static LogFun m_error;
    static LogFun m_critical;

public:
    inline static void init(const LogFun& a_info, 
                    const LogFun& a_warning, 
                    const LogFun& a_error, 
                    const LogFun& a_critical)                    
    {
        m_info = a_info;
        m_warning = a_warning;
        m_error = a_error;
        m_critical = a_critical;
    }

    inline void info(const std::string& a_message, const std::source_location& a_location)
    {
        if(m_info)
            m_info(a_message, a_location);
    }

    inline void warning(const std::string& a_message, const std::source_location& a_location)
    {
        if(m_warning)
            m_warning(a_message, a_location);
    }

    inline void error(const std::string& a_message, const std::source_location& a_location)
    {
        if(m_error)
            m_error(a_message, a_location);
    }

    inline void critical(const std::string& a_message, const std::source_location& a_location)
    {
        if(m_critical)
            m_critical(a_message, a_location);
    }
};