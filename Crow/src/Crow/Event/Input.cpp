#include "Input.h"
#include "../Application.h"
#include <memory>

namespace Crow {
	
	short int Input::s_Keys[];
	short int Input::s_MouseKeys[];
	Math::Vec2<int> Input::s_MousePos;

	void Input::Init()
	{
		memset(s_Keys, 0, sizeof(s_Keys)); // Set all keys to CROW_RELEASE
		memset(s_MouseKeys, 0, sizeof(s_MouseKeys));
	}
	
	void Input::KeyCallback(Math::Vec2<int> key)
	{
		s_Keys[key.x] = key.y;
		Application::OnEvent(KeyEvent(key));
	}

	void Input::MouseCallback(Math::Vec2<int> key)
	{
		s_MouseKeys[key.x] = key.y;
		Application::OnEvent(MouseEvent(key));
	}

	void Input::MousePosCallback(Math::Vec2<int> pos)
	{
		s_MousePos = pos;
		Application::OnEvent(MousePosEvent(pos));
	}
}