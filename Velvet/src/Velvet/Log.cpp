#include "vlpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Velvet {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init(spdlog::level::level_enum setLevel)
	{
		spdlog::set_pattern("%^[%T] %7n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("VELVET");
		s_CoreLogger->set_level(setLevel);
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(setLevel);
	}

}