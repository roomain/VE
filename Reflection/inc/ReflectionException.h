#pragma once
#include <source_location>
#include <exception>
#include <string>
#include <optional>
#include "ReflectionGlobals.h"


#pragma warning(push)
#pragma warning( disable : 4251 )
#pragma warning( disable : 4275 )


struct ParserInfo
{
	std::string filename;
	std::string currentProfile;
	std::string currentNode;
};

/*@brief Provides exception for vulkan*/
class REFLECTION_LIB ReflectionException : public std::exception
{
private:
	std::source_location m_srcLoc;
	std::optional<ParserInfo> m_parseInfo;

public:
	ReflectionException() = delete;
	explicit ReflectionException(const std::string& a_message, const std::source_location& a_loc = std::source_location::current());
	explicit ReflectionException(const std::string& a_message, const ParserInfo& a_info, const std::source_location& a_loc = std::source_location::current());
	std::string message()const noexcept;
};

#pragma warning(pop)