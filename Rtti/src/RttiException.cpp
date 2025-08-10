#include <format>
#include "RttiException.h"

RttiException::RttiException(RttiException::Type a_errType, std::string_view a_info) : m_exceptionType{ a_errType }
{
	genMessage(a_info);
}

RttiException::Type RttiException::type()const noexcept
{
	return m_exceptionType;
}

std::string RttiException::message()const noexcept
{
	return m_message;
}

void RttiException::genMessage(std::string_view info)
{
	if (info.empty())
	{
		switch (m_exceptionType)
		{
		case Type::parent_not_defined:
			m_message = "Parent Definition does not exist";
			break;

		case Type::null_definition:
			m_message = "Definition is null";
			break;

		case Type::already_defined:
			m_message = "Definition already exists";
			break;

		default:
			break;
		}
	}
	else
	{
		switch (m_exceptionType)
		{
		case Type::parent_not_defined:
			m_message = std::format("Parent ({}) definition does not exist", info);
			break;

		case Type::null_definition:
			m_message = std::format("{} definition is null", info);
			break;

		case Type::already_defined:
			m_message = std::format("{} definition already exists", info);
			break;

		default:
			break;
		}
	}
}

const char* RttiException::what()const
{
	return m_message.c_str();
}