#include "pch.h"
#include <format>

ReflectionException::ReflectionException(const std::string& a_message, const std::source_location& a_loc) :
	std::exception(a_message.c_str()), m_srcLoc{ a_loc }
{
	//
}

ReflectionException::ReflectionException(const std::string& a_message, const ParserInfo& a_info, const std::source_location& a_loc) :
	std::exception(a_message.c_str()), m_srcLoc{ a_loc }, m_parseInfo{ a_info }
{
	//
}

std::string ReflectionException::message()const noexcept
{
	if (m_parseInfo.has_value())
	{
		return std::format("Error: File: {}\nLine: {}\nFunction: {}\nDescription: {}\n" \
			"During parsing of file {} at node {} with profile {}\n",
			m_srcLoc.file_name(), m_srcLoc.line(), m_srcLoc.function_name(),
			what(), m_parseInfo.value().filename, m_parseInfo.value().currentNode, m_parseInfo.value().currentProfile);
	}
	
	return std::format("Error: File: {}\nLine: {}\nFunction: {}\nDescription: {}\n",
		m_srcLoc.file_name(), m_srcLoc.line(), m_srcLoc.function_name(),
		what());
}