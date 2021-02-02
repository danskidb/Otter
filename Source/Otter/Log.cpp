#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Otter {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("Otter");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	void Log::CoreAssert(bool condition, std::string errorMsg)
	{
		if (!condition)
		{
			Otter::Log::GetCoreLogger()->error(errorMsg);
			assert(condition);
		}
	}
}