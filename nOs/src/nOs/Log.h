#pragma once
#pragma warning(push)
#pragma warning(disable: 4251)

#include "nospch.h"
#include "Core.h"
#include "Logger.h"


namespace nOs {
	class NOS_API Log {
	public:
		static void Init();

		inline static std::shared_ptr<Logger>& GetCoreLogger() {
			return s_CoreLogger;
		}

		inline static std::shared_ptr<Logger>& GetClientLogger() {
			return s_ClientLogger;
		}
	private:
		static std::shared_ptr<Logger> s_CoreLogger;
		static std::shared_ptr<Logger> s_ClientLogger;
	};
}

#pragma warning(pop)

//Core log macros
#define NOS_CORE_TRACE(...)	::nOs::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NOS_CORE_DEBUG(...)	::nOs::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define NOS_CORE_INFO(...)	::nOs::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NOS_CORE_WARN(...)	::nOs::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NOS_CORE_ERROR(...)	::nOs::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NOS_CORE_FATAL(...)	::nOs::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define NOS_TRACE(...)		::nOs::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NOS_DEBUG(...)		::nOs::Log::GetClientLogger()->debug(__VA_ARGS__)
#define NOS_INFO(...)		::nOs::Log::GetClientLogger()->info(__VA_ARGS__)
#define NOS_WARN(...)		::nOs::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NOS_ERROR(...)		::nOs::Log::GetClientLogger()->error(__VA_ARGS__)
#define NOS_FATAL(...)		::nOs::Log::GetClientLogger()->fatal(__VA_ARGS__)