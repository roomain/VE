#pragma once
/***********************************************
* @headerfile iLogDisplayer.h
* @date 16 / 08 / 2024
* @author Roomain
************************************************/
#include "logData.h"
#include "bitOperators.h"

/*@brief interface for log displayer*/
class ILogDisplayer
{
private:
    LogSeverity m_filter = LogSeverity::Info | LogSeverity::Warning | LogSeverity::Error | LogSeverity::Critical;

protected:
    virtual void intern_process(const LogData& a_logData) = 0;

public:
    explicit ILogDisplayer(LogSeverity a_filter) : m_filter{ a_filter } {}
    ILogDisplayer() = default;
    virtual ~ILogDisplayer() = default;
    inline void processLog(const LogData& a_logData)
    {
        if ((a_logData.severity & m_filter) != LogSeverity::None)
        {
            intern_process(a_logData);
        }
    }
};
