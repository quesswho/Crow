#pragma once

#include <glm/glm.hpp>

namespace Crow {

	struct Rectangle {
		glm::vec2 m_Position, m_Size;

		Rectangle()
			: m_Size(0.0f,0.0f), m_Position(0.0f)
		{}

		Rectangle(glm::vec2 size, glm::vec2 pos)
			: m_Size(size), m_Position(pos)
		{}

		bool IsCollide(Rectangle* rect) const {
			return (m_Position.x + m_Size.x >= rect->m_Position.x &&
					rect->m_Position.x + rect->m_Size.x >= m_Position.x) &&
					(m_Position.y + m_Size.y >= rect->m_Position.y &&
					rect->m_Position.y + rect->m_Size.y >= m_Position.y);
		}

		glm::vec2 GetCorrection(Rectangle* rect) const {
			if (IsCollide(rect))
			{
				//return glm::vec2(abs((m_Position.x + m_Size.x) - (rect->m_Position.x + rect->m_Size.x)+0.01f), abs((m_Position.y + m_Size.y) - (rect->m_Position.y + rect->m_Size.y) + 0.01f));
				return glm::vec2(0.0f, 0.0f);
			}
		}
	};
}