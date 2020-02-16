#include "Log.h"

namespace Crow {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_GameLogger;

	void Log::Init()
	{
#ifdef CR_DEBUG
		spdlog::set_pattern("%^[%H:%M:%S][%n][%l]: %v%$");
		s_CoreLogger = spdlog::stderr_color_mt("Core");
		s_GameLogger = spdlog::stderr_color_mt("Game");
#endif
	}
}