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
