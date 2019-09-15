#pragma once

#include <chrono>

namespace Crow {

	class Timer {
	public:
		Timer()
			: m_LastElapsed(0.0f)
		{}

		void Start() { m_End = std::chrono::high_resolution_clock::now();  }
		void End() { m_Now = std::chrono::high_resolution_clock::now(); }

		const float GetElapsedTimeInSeconds() { m_LastElapsed = std::chrono::duration<float>(m_Now - m_End).count(); return m_LastElapsed; }
		inline const float GetLastElapsed() const { return m_LastElapsed; }
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Now;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_End;

		float m_LastElapsed;
	};
}