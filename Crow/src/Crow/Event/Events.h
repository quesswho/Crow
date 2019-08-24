#pragma once

#include "Input.h"

namespace Crow {

	enum EventType {
		KEY,
		MOUSE,
		MOUSEPOS
	};

	struct vec2 {
		int m_X;
		int m_Y;
		vec2(int x, int y)
			: m_X(x), m_Y(y)
		{}
	};


	// 3 virtual return functions to avoid std::dynamic_cast
	struct Event {
		EventType m_Type;
		Event(EventType type)
			: m_Type(type)
		{}
		virtual std::tuple<int, int> getKeyValue() = 0;
	};

	struct KeyEvent : public Event {
		std::tuple<int, int> m_Key;
		KeyEvent(std::tuple<int, int> key)
			: Event(KEY), m_Key(key)
		{}
		inline std::tuple<int, int> getKeyValue() override { return m_Key; }
	};

	struct MouseEvent : public Event {
		std::tuple<int, int> m_Key;
		MouseEvent(std::tuple<int, int> key)
			: Event(MOUSE), m_Key(key)
		{}
		inline std::tuple<int, int> getKeyValue() override { return m_Key; }
	};

	struct MousePosEvent : public Event {
		std::tuple<int, int> m_Vec;
		MousePosEvent(std::tuple<int, int> vec)
			: Event(MOUSEPOS), m_Vec(vec)
		{}
		inline std::tuple<int, int> getKeyValue() override { return m_Vec; }
	};

}