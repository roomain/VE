#pragma once
/***********************************************
* @headerfile LoggerDisplay.h
* @date 30 / 07 / 2025
* @author Roomain
************************************************/
#include <iostream>
#include "iLogDisplayer.h"

class LoggerDisplay : public ILogDisplayer
{
protected:
	void intern_process(const LogData& a_logData) override
	{
		std::cout << a_logData.location.file_name() << " ";
		std::cout << a_logData.location.function_name() << " ";
		std::cout << a_logData.location.line() << " ";
		std::cout << a_logData.message << std::endl;
	}

public:
	using ILogDisplayer::ILogDisplayer;
};
