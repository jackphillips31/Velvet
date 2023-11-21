#pragma once

#include "Velvet/Core/Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/fmt.h>

namespace Velvet {

	class VELVET_API Log
	{
	public:
		static void Init(spdlog::level setLevel);

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Add VL_LOG_LOCATION to Preprocessor Definitions
// to enable printing the source location information 
// from which the log is called.

namespace LogDebug {
	template<typename T>
	inline static std::string ToString(T& t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}
	template<typename... Args>
	inline static std::string ToString(const std::string& fmt, Args&&... args)
	{
		return fmt::format(fmt, std::forward<Args>(args)...);
	}
	inline static std::string ExtractFileName(const char* fullFilePath)
	{
		std::string filePath(fullFilePath);
		size_t pos = filePath.find_last_of("/\\");
		if (pos != std::string::npos)
		{
			return filePath.substr(pos + 1);
		}
		return filePath;
	}
	inline static std::string FormatFileAndLine(std::string fileName, int line)
	{
		return fmt::format("({}:{})", fileName, line);
	}
}

#ifdef VL_LOG_LOCATION
	#define VL_CORE_CRITICAL(...) ::Velvet::Log::GetCoreLogger()->critical("{:^30} {:>30}: {}",__FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define VL_CORE_ERROR(...) ::Velvet::Log::GetCoreLogger()->error("{:^30} {:>30}: {}",__FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define VL_CORE_WARN(...) ::Velvet::Log::GetCoreLogger()->warn("{:^30} {:>30}: {}",__FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define VL_CORE_INFO(...) ::Velvet::Log::GetCoreLogger()->info("{:^30} {:>30}: {}",__FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define VL_CORE_BUG(...) ::Velvet::Log::GetCoreLogger()->debug("{:^30} {:>30}: {}",__FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define VL_CORE_TRACE(...) ::Velvet::Log::GetCoreLogger()->trace("{:^30} {:>30}: {}",__FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))

	#define VL_CRITICAL(...) ::Velvet::Log::GetClientLogger()->critical("{:^30} {:>30}: {}",__FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define VL_ERROR(...) ::Velvet::Log::GetClientLogger()->error("{:^30} {:>30}: {}",__FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define VL_WARN(...) ::Velvet::Log::GetClientLogger()->warn("{:^30} {:>30}: {}",__FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define VL_INFO(...) ::Velvet::Log::GetClientLogger()->info("{:^30} {:>30}: {}",__FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define VL_BUG(...) ::Velvet::Log::GetClientLogger()->debug("{:^30} {:>30}: {}",__FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define VL_TRACE(...) ::Velvet::Log::GetClientLogger()->trace("{:^30} {:>30}: {}",__FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
#else
	#define VL_CORE_CRITICAL(...) ::Velvet::Log::GetCoreLogger()->critical(LogDebug::ToString(__VA_ARGS__))
	#define VL_CORE_ERROR(...) ::Velvet::Log::GetCoreLogger()->error(LogDebug::ToString(__VA_ARGS__))
	#define VL_CORE_WARN(...) ::Velvet::Log::GetCoreLogger()->warn(LogDebug::ToString(__VA_ARGS__))
	#define VL_CORE_INFO(...) ::Velvet::Log::GetCoreLogger()->info(LogDebug::ToString(__VA_ARGS__))
	#define VL_CORE_TRACE(...) ::Velvet::Log::GetCoreLogger()->trace(LogDebug::ToString(__VA_ARGS__))

	#define VL_CRITICAL(...) ::Velvet::Log::GetClientLogger()->critical(LogDebug::ToString(__VA_ARGS__))
	#define VL_ERROR(...) ::Velvet::Log::GetClientLogger()->error(LogDebug::ToString(__VA_ARGS__))
	#define VL_WARN(...) ::Velvet::Log::GetClientLogger()->warn(LogDebug::ToString(__VA_ARGS__))
	#define VL_INFO(...) ::Velvet::Log::GetClientLogger()->info(LogDebug::ToString(__VA_ARGS__))
	#define VL_TRACE(...) ::Velvet::Log::GetClientLogger()->trace(LogDebug::ToString(__VA_ARGS__))
#endif