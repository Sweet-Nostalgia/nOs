#include "Logger.h"

// ANSI escape codes
#define RESET   "\033[0m"
#define RED     "\033[91m"
#define YELLOW  "\033[93m"
#define GREEN   "\033[92m"
#define BLUE    "\033[94m"
#define MAGENTA "\033[95m"
#define WHITE   "\033[97m"

Logger::Logger(const std::string& name): m_Name(name) {

}

std::shared_ptr<Logger> Logger::create(const std::string& name) {
	return std::shared_ptr<Logger>(new Logger(name));
}

void Logger::log(LogLevel level, const std::string& message) {
	if (level < m_Level) {
		return;
	}

	std::string time = getTime();

	std::ostringstream logEntry;
	logEntry << "[" << time << "] " << levelToString(level) << ": " << message << std::endl;

	//Output color based on level
	switch (level) {
	case LogLevel::Trace:   std::cout << WHITE << logEntry.str() << RESET; break;
	case LogLevel::Debug:   std::cout << BLUE << logEntry.str() << RESET; break;
	case LogLevel::Info:    std::cout << GREEN << logEntry.str() << RESET; break;
	case LogLevel::Warn:    std::cout << YELLOW << logEntry.str() << RESET; break;
	case LogLevel::Error:   std::cout << RED << logEntry.str() << RESET; break;
	case LogLevel::Fatal:   std::cout << MAGENTA << logEntry.str() << RESET; break;
	}
}

std::string Logger::formatBrackets(const std::string& format, const std::vector<std::string>& argList) {
	std::string result = format;

	for (size_t i = 0; i < argList.size(); ++i) {
		std::string placeholder = "{" + std::to_string(i) + "}";
		size_t pos = result.find(placeholder);

		while (pos != std::string::npos) {
			result.replace(pos, placeholder.length(), argList[i]);
			pos = result.find(placeholder, pos + argList[i].length());
		}
	}

	return result;
}

std::string Logger::getTime() {
	std::time_t now = std::time(nullptr);
	std::tm timeinfo;
	localtime_s(&timeinfo, &now); // Safe Windows version
	char buffer[20];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
	return std::string(buffer);
}

std::string Logger::levelToString(LogLevel level) {
	switch (level) {
	case LogLevel::Trace:	return "TRACE";
	case LogLevel::Debug:	return "DEBUG";
	case LogLevel::Info:	return "INFO";
	case LogLevel::Warn:	return "WARN";
	case LogLevel::Error:	return "ERROR";
	case LogLevel::Fatal:	return "FATAL";
	default:				return "UNKNOWN";
	}
}