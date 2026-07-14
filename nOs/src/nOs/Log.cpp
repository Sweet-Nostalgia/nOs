#include "Log.h"
#include "Logger.h"

namespace nOs {
	std::shared_ptr<Logger> Log::s_CoreLogger;
	std::shared_ptr<Logger> Log::s_ClientLogger;

	void Log::Init() { 
		s_CoreLogger = Logger::create("nOs");
		s_CoreLogger->trace("Core Logger Initialized");
		s_ClientLogger = Logger::create("App");
		s_ClientLogger->trace("Client Logger Initialized");
	}
}