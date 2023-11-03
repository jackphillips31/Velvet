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

#ifdef VL_ENABLE_ASSERTS
	#define VL_ASSERT(x, ...) { if(!(x)) { VL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define VL_CORE_ASSERT(x, ...) { if(!(x)) { VL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define VL_ASSERT(x, ...)
	#define VL_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)