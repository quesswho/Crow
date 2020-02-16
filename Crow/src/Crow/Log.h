#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

#include <memory>
#include "Common.h"

namespace Crow {

	class Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getGameLogger() { return s_GameLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_GameLogger;
	};
}




