#include "pch.h"
#include <ranges>
#include <utility>
#include <functional>
#include "logger.h"
#include "iLogDisplayer.h"


void Logger::dispatchThreadFun()
{
	std::mutex condProtection;
	while (m_bRunDispatch)
	{
		std::unique_lock dispatchProtect(condProtection);
		m_dispatchCondition.wait(dispatchProtect, [this]() {return m_bDispatch; });

		std::scoped_lock lockDispatch(m_dispatchProtection);
		m_bDispatch = false; // reset
		for (const auto& log : m_logBuffer)
		{
			for (const auto& displayer : m_displayers)
				displayer->processLog(log);
		}
	}
}

Logger& Logger::instance()
{
	static Logger Instance;
	return Instance;
}

Logger::Logger()
{
	m_dispatchThread = std::move(std::thread(std::bind(&Logger::dispatchThreadFun, this)));
}

Logger::~Logger()
{
	m_bRunDispatch = false;
	m_dispatchThread.join();
}

void Logger::addLogDisplayer(ILogDisplayerPtr a_displayer)
{
	std::scoped_lock lockDispatch(m_dispatchProtection);
	m_displayers.emplace_back(a_displayer);
}

void Logger::dispatch(LogData&& a_log)noexcept
{
	std::scoped_lock lockDispatch(m_dispatchProtection);
	m_logBuffer.emplace_back(a_log);
	if (m_logBuffer.size() >= m_logLimit)
	{
		m_bDispatch = true;
		m_dispatchCondition.notify_one();
	}
}

void Logger::setLogLimit(const size_t& a_limit)
{
	std::scoped_lock lockDispatch(Logger::instance().m_dispatchProtection);
	Logger::instance().m_logLimit = a_limit;
}

void Logger::info(const std::string& a_message, const std::source_location& a_location)
{
	Logger::instance().dispatch(LogData{ .severity = LogSeverity::Info,
		.location = a_location, .message = a_message });
}


void Logger::warning(const std::string& a_message, const std::source_location& a_location)
{
	Logger::instance().dispatch(LogData{ .severity = LogSeverity::Warning,
		.location = a_location, .message = a_message });
}


void Logger::error(const std::string& a_message, const std::source_location& a_location)
{
	Logger::instance().dispatch(LogData{ .severity = LogSeverity::Error,
		.location = a_location, .message = a_message });
}


void Logger::critical(const std::string& a_message, const std::source_location& a_location)
{
	Logger::instance().dispatch(LogData{ .severity = LogSeverity::Critical,
		.location = a_location, .message = a_message });
}
