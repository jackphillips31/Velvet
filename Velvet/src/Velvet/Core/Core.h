#pragma once

#include <memory>

#ifdef VL_PLATFORM_WINDOWS
	#define VELVET_API
#else
	#error Velvet only supports Windows
#endif

#ifdef VL_DEBUG
	#define VL_ENABLE_ASSERTS
#endif

#ifdef VL_ENABLE_ASSERTS
	#define VL_ASSERT(x, ...) { if(!(x)) { VL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define VL_CORE_ASSERT(x, ...) { if(!(x)) { VL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define VL_ASSERT(x, ...)
	#define VL_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
#define VL_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Velvet {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}