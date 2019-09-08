#pragma once

#include <chrono>

namespace Crow {

	class Timer {
	public:
		Timer()
		{}

		void Start() { m_End = std::chrono::high_resolution_clock::now();  }
		void End() { m_Now = std::chrono::high_resolution_clock::now(); }

		double GetElapsedTimeInSeconds() const { return std::chrono::duration<double>(m_Now - m_End).count();  }
	public:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Now;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_End;
	};
}