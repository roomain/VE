#pragma once
/***********************************************
* @headerfile Exception.h
* @date 24 / 08 / 2024
* @author Roomain
************************************************/
#include <source_location>
#include "logData.h"

/*@brief base class for loggable exception*/
class Exception
{
protected:
	LogData m_logData;

public:
	Exception() = delete;
	Exception(const std::string_view a_message, const std::source_location& a_loc = std::source_location::current())
	{
		m_logData.message = a_message;
		m_logData.location = a_loc;
		m_logData.severity = LogSeverity::Error;
	}
	virtual ~Exception() = default;
	[[nodiscard]] LogData logData()const noexcept { return m_logData; }
};
