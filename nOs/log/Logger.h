#pragma once

#include "nospch.h"
#include "nOs/Core.h"

enum LogLevel {
	Trace,
	Debug,
	Info,
	Warn,
	Error,
	Fatal
};

class NOS_API Logger {
public:
	static std::shared_ptr<Logger> create(const std::string& name);

	void set_level(LogLevel level) {
		m_Level = level;
	}

	LogLevel get_level() const {
		return m_Level;
	}

	template<typename... Args>
	void trace(const std::string& format, Args&&... args) {
		if (m_Level <= LogLevel::Trace) {
			log(LogLevel::Trace, formatBrackets(format, { formatArgument(args)... }));
		}
	}

	template<typename... Args>
	void debug(const std::string& format, Args&&... args) {
		if (m_Level <= LogLevel::Debug) {
			log(LogLevel::Debug, formatBrackets(format, { formatArgument(args)... }));
		}
	}

	template<typename... Args>
	void info(const std::string& format, Args&&... args) {
		if (m_Level <= LogLevel::Info) {
			log(LogLevel::Info, formatBrackets(format, { formatArgument(args)... }));
		}
	}

	template<typename... Args>
	void warn(const std::string& format, Args&&... args) {
		if (m_Level <= LogLevel::Warn) {
			log(LogLevel::Warn, formatBrackets(format, { formatArgument(args)... }));
		}
	}

	template<typename... Args>
	void error(const std::string& format, Args&&... args) {
		if (m_Level <= LogLevel::Error) {
			log(LogLevel::Error, formatBrackets(format, { formatArgument(args)... }));
		}
	}

	template<typename... Args>
	void fatal(const std::string& format, Args&&... args) {
		if (m_Level <= LogLevel::Fatal) {
			log(LogLevel::Fatal, formatBrackets(format, { formatArgument(args)... }));
		}
	}

private:
	Logger(const std::string& name);
	void log(LogLevel level, const std::string& message);
	std::string getTime();
	std::string levelToString(LogLevel level);

	template<typename T>
	std::string formatArgument(T&& arg) {
		std::ostringstream ss;
		ss << arg;
		return ss.str();
	}

	std::string formatBrackets(const std::string& format, const std::vector<std::string>& argList);

	std::string m_Name;
	LogLevel m_Level = LogLevel::Trace;
};