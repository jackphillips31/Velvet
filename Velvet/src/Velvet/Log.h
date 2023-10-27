#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Velvet {

	class VELVET_API Log
	{
	public:
		static void Init(spdlog::level::level_enum setLevel);

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		inline static std::string ExtractFileName(const char* fullFilePath);

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

#ifdef VL_LOG_LOCATION
	#define VL_CORE_CRITICAL(msg, ...) ::Velvet::Log::GetCoreLogger()->critical("{} ({}:{}): " msg, ::Velvet::Log::ExtractFileName(__FILE__), __FUNCTION__, __LINE__, __VA_ARGS__)
	#define VL_CORE_ERROR(msg, ...) ::Velvet::Log::GetCoreLogger()->error("{} ({}:{}): " msg, ::Velvet::Log::ExtractFileName(__FILE__), __FUNCTION__, __LINE__, __VA_ARGS__)
	#define VL_CORE_WARN(msg, ...) ::Velvet::Log::GetCoreLogger()->warn("{} ({}:{}): " msg, ::Velvet::Log::ExtractFileName(__FILE__), __FUNCTION__, __LINE__, __VA_ARGS__)
	#define VL_CORE_INFO(msg, ...) ::Velvet::Log::GetCoreLogger()->info("{} ({}:{}): " msg, ::Velvet::Log::ExtractFileName(__FILE__), __FUNCTION__, __LINE__, __VA_ARGS__)
	#define VL_CORE_DEBUG(msg, ...) ::Velvet::Log::GetCoreLogger()->debug("{} ({}:{}): " msg, ::Velvet::Log::ExtractFileName(__FILE__), __FUNCTION__, __LINE__, __VA_ARGS__)
	#define VL_CORE_TRACE(msg, ...) ::Velvet::Log::GetCoreLogger()->trace("{} ({}:{}): " msg, ::Velvet::Log::ExtractFileName(__FILE__), __FUNCTION__, __LINE__, __VA_ARGS__)

	#define VL_CRITICAL(msg, ...) ::Velvet::Log::GetClientLogger()->critical("{} ({}:{}): " msg, ::Velvet::Log::ExtractFileName(__FILE__), __FUNCTION__, __LINE__, __VA_ARGS__)
	#define VL_ERROR(msg, ...) ::Velvet::Log::GetClientLogger()->error("{} ({}:{}): " msg, ::Velvet::Log::ExtractFileName(__FILE__), __FUNCTION__, __LINE__, __VA_ARGS__)
	#define VL_WARN(msg, ...) ::Velvet::Log::GetClientLogger()->warn("{} ({}:{}): " msg, ::Velvet::Log::ExtractFileName(__FILE__), __FUNCTION__, __LINE__, __VA_ARGS__)
	#define VL_INFO(msg, ...) ::Velvet::Log::GetClientLogger()->info("{} ({}:{}): " msg, ::Velvet::Log::ExtractFileName(__FILE__), __FUNCTION__, __LINE__, __VA_ARGS__)
	#define VL_DEBUG(msg, ...) ::Velvet::Log::GetClientLogger()->debug("{} ({}:{}): " msg, ::Velvet::Log::ExtractFileName(__FILE__), __FUNCTION__, __LINE__, __VA_ARGS__)
	#define VL_TRACE(msg, ...) ::Velvet::Log::GetClientLogger()->trace("{} ({}:{}): " msg, ::Velvet::Log::ExtractFileName(__FILE__), __FUNCTION__, __LINE__, __VA_ARGS__)
#else
	#define VL_CORE_CRITICAL(...) ::Velvet::Log::GetCoreLogger()->critical(__VA_ARGS__)
	#define VL_CORE_ERROR(...) ::Velvet::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define VL_CORE_WARN(...) ::Velvet::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define VL_CORE_INFO(...) ::Velvet::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define VL_CORE_DEBUG(...) ::Velvet::Log::GetCoreLogger()->debug(__VA_ARGS__)
	#define VL_CORE_TRACE(...) ::Velvet::Log::GetCoreLogger()->trace(__VA_ARGS__)

	#define VL_CRITICAL(...) ::Velvet::Log::GetClientLogger()->critical(__VA_ARGS__)
	#define VL_ERROR(...) ::Velvet::Log::GetClientLogger()->error(__VA_ARGS__)
	#define VL_WARN(...) ::Velvet::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define VL_INFO(...) ::Velvet::Log::GetClientLogger()->info(__VA_ARGS__)
	#define VL_DEBUG(...) ::Velvet::Log::GetClientLogger()->debug(__VA_ARGS__)
	#define VL_TRACE(...) ::Velvet::Log::GetClientLogger()->trace(__VA_ARGS__)
#endif