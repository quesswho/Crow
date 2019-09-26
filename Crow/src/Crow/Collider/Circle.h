#pragma once

#include <glm/glm.hpp>

namespace Crow {

	struct Circle {
		glm::vec2 m_Position, m_Size;

		Circle()
			: m_Size(0.0f, 0.0f), m_Position(0.0f)
		{}

		Circle(glm::vec2 size, glm::vec2 pos)
			: m_Size(size), m_Position(pos)
		{}

		bool IsCollide(Circle* rect) const {
			return (m_Position.x + m_Size.x >= rect->m_Position.x &&
				rect->m_Position.x + rect->m_Size.x >= m_Position.x) &&
				(m_Position.y + m_Size.y >= rect->m_Position.y &&
					rect->m_Position.y + rect->m_Size.y >= m_Position.y);
		}

		glm::vec2 GetCorrection(Circle* rect) const {
			if (IsCollide(rect))
			{
				return glm::vec2(m_Position.x + m_Size - rect->m_Position.x + rect->m_Size, 0.0f);
			}
		}
	};
}