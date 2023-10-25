#pragma once

#ifdef VL_PLATFORM_WINDOWS
	#ifdef VL_BUILD_DLL
		#define VELVET_API __declspec(dllexport)
	#else
		#define VELVET_API __declspec(dllimport)
	#endif
#else
	#error Velvet only supports Windows
#endif