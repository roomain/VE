#pragma once
/***********************************************
* @headerfile RttiException.h
* @date 04 / 04 / 2025
* @author Romain Crespel
************************************************/
#include <exception>
#include <string>

/*@brief Rtti exception class*/
class RttiException : public std::exception
{
public:
	enum class Type : int
	{
		parent_not_defined, /* @brief parent class is not RTTI defined*/
		null_definition,	/* @brief class is not defined*/
		already_defined		/* @brief class RTTI is already defined*/
	};

private:
	Type			m_exceptionType;
	std::string		m_message;

	void genMessage(std::string_view info);

public:
	RttiException(RttiException::Type a_errType, std::string_view a_info = "");
	RttiException() = delete;
	virtual ~RttiException() = default;
	[[nodiscard]] Type type()const noexcept;
	[[nodiscard]] std::string message()const noexcept;
	const char* what()const override;/*!< NOT USED*/
};

