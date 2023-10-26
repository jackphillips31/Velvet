#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Velvet {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init(spdlog::level::level_enum setLevel)
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("VELVET");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(setLevel);
	}

	std::string Log::ExtractFileName(const char* fullFilePath)
	{
		std::string filePath(fullFilePath);
		size_t pos = filePath.find_last_of("/\\");
		if (pos != std::string::npos) {
			return filePath.substr(pos + 1);
		}
		return filePath;
	}

}