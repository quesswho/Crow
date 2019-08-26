#include "Log.h"
#include "spdlog/sinks/stdout_sinks.h"
//#include <spdlog/sinks/stdout_color_sinks.h>

#include <type_traits>

namespace Crow {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_GameLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%H:%M:%S][%n][%l]: %v%$");
		s_CoreLogger = spdlog::stderr_color_mt("Core");
		s_GameLogger = spdlog::stderr_color_mt("Game");
	}
}