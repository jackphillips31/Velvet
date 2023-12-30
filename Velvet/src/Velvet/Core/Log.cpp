#include <vlpch.h>
#include "Velvet/Core/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Velvet {

	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;

	void Log::Init(spdlog::level setLevel)
	{
		spdlog::set_pattern("%^[%T] %7n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("VELVET");
		s_CoreLogger->set_level(setLevel);
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(setLevel);
		VL_CORE_INFO("Logging Initialized");
	}

}