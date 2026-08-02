#pragma once

#ifdef NOS_PLATFORM_WINDOWS
	#ifdef NOS_BUILD_DLL
		#define NOS_API __declspec(dllexport)
	#else
		#define NOS_API __declspec(dllimport)
	#endif
#else
	#error nOs only supports Windows!
#endif

#ifdef NOS_ENABLE_ASSERTS
	#define NOS_ASSERT(x, ...) { \
		if(!(x)) { \
			NOS_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
			__debugbreak(); \
		} \
	}
	#define NOS_CORE_ASSERT(x, ...) { \
		if(!(x)) { \
			NOS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
			__debugbreak(); \
		} \
	}
#else
	#define NOS_ASSERT(x, ...)
	#define NOS_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)