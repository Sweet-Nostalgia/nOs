#include "Log.h"
#include "Logger.h"

namespace nOs {
	std::shared_ptr<Logger> Log::s_CoreLogger;
	std::shared_ptr<Logger> Log::s_ClientLogger;
	
	void Log::Init() { 
		s_CoreLogger = Logger::create("nOs");
		s_ClientLogger = Logger::create("App");
	}
}