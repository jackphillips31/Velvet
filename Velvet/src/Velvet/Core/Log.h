#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/fmt.h>

namespace Velvet {

	class VELVET_API Log
	{
	public:
		static void Init(spdlog::level::level_enum setLevel);

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
#ifdef VL_LOG_LOCATION
	namespace LogLocationDebug {
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
			if (pos != std::string::npos) {
				return filePath.substr(pos + 1);
			}
			return filePath;
		}
		inline static std::string FormatFileAndLine(std::string fileName, int line)
		{
			return fmt::format("({}:{})", fileName, line);
		}
	}

	#define VL_CORE_CRITICAL(...) ::Velvet::Log::GetCoreLogger()->critical("{:^30} {:>30}: {}",__FUNCTION__, LogLocationDebug::FormatFileAndLine(LogLocationDebug::ExtractFileName(__FILE__), __LINE__), LogLocationDebug::ToString(__VA_ARGS__))
	#define VL_CORE_ERROR(...) ::Velvet::Log::GetCoreLogger()->error("{:^30} {:>30}: {}",__FUNCTION__, LogLocationDebug::FormatFileAndLine(LogLocationDebug::ExtractFileName(__FILE__), __LINE__), LogLocationDebug::ToString(__VA_ARGS__))
	#define VL_CORE_WARN(...) ::Velvet::Log::GetCoreLogger()->warn("{:^30} {:>30}: {}",__FUNCTION__, LogLocationDebug::FormatFileAndLine(LogLocationDebug::ExtractFileName(__FILE__), __LINE__), LogLocationDebug::ToString(__VA_ARGS__))
	#define VL_CORE_INFO(...) ::Velvet::Log::GetCoreLogger()->info("{:^30} {:>30}: {}",__FUNCTION__, LogLocationDebug::FormatFileAndLine(LogLocationDebug::ExtractFileName(__FILE__), __LINE__), LogLocationDebug::ToString(__VA_ARGS__))
	#define VL_CORE_DEBUG(...) ::Velvet::Log::GetCoreLogger()->debug("{:^30} {:>30}: {}",__FUNCTION__, LogLocationDebug::FormatFileAndLine(LogLocationDebug::ExtractFileName(__FILE__), __LINE__), LogLocationDebug::ToString(__VA_ARGS__))
	#define VL_CORE_TRACE(...) ::Velvet::Log::GetCoreLogger()->trace("{:^30} {:>30}: {}",__FUNCTION__, LogLocationDebug::FormatFileAndLine(LogLocationDebug::ExtractFileName(__FILE__), __LINE__), LogLocationDebug::ToString(__VA_ARGS__))

	#define VL_CRITICAL(...) ::Velvet::Log::GetClientLogger()->critical("{:^30} {:>30}: {}",__FUNCTION__, LogLocationDebug::FormatFileAndLine(LogLocationDebug::ExtractFileName(__FILE__), __LINE__), LogLocationDebug::ToString(__VA_ARGS__))
	#define VL_ERROR(...) ::Velvet::Log::GetClientLogger()->error("{:^30} {:>30}: {}",__FUNCTION__, LogLocationDebug::FormatFileAndLine(LogLocationDebug::ExtractFileName(__FILE__), __LINE__), LogLocationDebug::ToString(__VA_ARGS__))
	#define VL_WARN(...) ::Velvet::Log::GetClientLogger()->warn("{:^30} {:>30}: {}",__FUNCTION__, LogLocationDebug::FormatFileAndLine(LogLocationDebug::ExtractFileName(__FILE__), __LINE__), LogLocationDebug::ToString(__VA_ARGS__))
	#define VL_INFO(...) ::Velvet::Log::GetClientLogger()->info("{:^30} {:>30}: {}",__FUNCTION__, LogLocationDebug::FormatFileAndLine(LogLocationDebug::ExtractFileName(__FILE__), __LINE__), LogLocationDebug::ToString(__VA_ARGS__))
	#define VL_DEBUG(...) ::Velvet::Log::GetClientLogger()->debug("{:^30} {:>30}: {}",__FUNCTION__, LogLocationDebug::FormatFileAndLine(LogLocationDebug::ExtractFileName(__FILE__), __LINE__), LogLocationDebug::ToString(__VA_ARGS__))
	#define VL_TRACE(...) ::Velvet::Log::GetClientLogger()->trace("{:^30} {:>30}: {}",__FUNCTION__, LogLocationDebug::FormatFileAndLine(LogLocationDebug::ExtractFileName(__FILE__), __LINE__), LogLocationDebug::ToString(__VA_ARGS__))
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