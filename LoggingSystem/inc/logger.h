#pragma once

/***********************************************
* @headerfile logger.h
* @date 16 / 08 / 2024
* @author Roomain
************************************************/
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>	
#include <memory>
#include <source_location>
#include "notCopiable.h"

struct LogData;
class ILogDisplayer;

using ILogDisplayerPtr = std::shared_ptr<ILogDisplayer>;

/*@brief logger wich dispatch logs to displayers*/
class Logger
{
private:
	std::vector<LogData> m_logBuffer;				/*!< buffer of log (stamp memory)*/
	std::vector<ILogDisplayerPtr> m_displayers;		/*!< log displayers*/
	size_t m_logLimit = 1;							/*!< buffer size for dispatch*/

	std::thread m_dispatchThread;
	std::condition_variable m_dispatchCondition;
	std::mutex m_dispatchProtection;
	std::atomic_bool m_bRunDispatch = true;
	bool m_bDispatch = false;

	void dispatchThreadFun();

	Logger();

	void dispatch(LogData&& a_log)noexcept;

public:
	static Logger& instance();
	NOT_COPIABLE(Logger)
	~Logger();
	void addLogDisplayer(ILogDisplayerPtr a_displayer);
	static void setLogLimit(const size_t& a_limit);
	static void info(const std::string& a_message, const std::source_location& a_location = std::source_location::current());
	static void warning(const std::string& a_message, const std::source_location& a_location = std::source_location::current());
	static void error(const std::string& a_message, const std::source_location& a_location = std::source_location::current());
	static void critical(const std::string& a_message, const std::source_location& a_location = std::source_location::current());
};