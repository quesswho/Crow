#pragma once

#include "Input.h"

namespace Crow {

	enum EventType {
		KEY,
		MOUSE,
		MOUSEPOS
	};

	// 3 virtual return functions to avoid std::dynamic_cast
	struct Event {
		EventType m_Type;
		Event(EventType type)
			: m_Type(type)
		{}
		virtual Math::Vec2<int> GetKey() = 0;
	};

	struct KeyEvent : public Event {
		Math::Vec2<int> m_Key;
		KeyEvent(Math::Vec2<int> key)
			: Event(KEY), m_Key(key)
		{}
		inline Math::Vec2<int> GetKey() override { return m_Key; }
	};

	struct MouseEvent : public Event {
		Math::Vec2<int> m_Key;
		MouseEvent(Math::Vec2<int> key)
			: Event(MOUSE), m_Key(key)
		{}
		inline Math::Vec2<int> GetKey() override { return m_Key; }
	};

	struct MousePosEvent : public Event {
		Math::Vec2<int> m_Vec;
		MousePosEvent(Math::Vec2<int> vec)
			: Event(MOUSEPOS), m_Vec(vec)
		{}
		inline Math::Vec2<int> GetKey() override { return m_Vec; }
	};

}