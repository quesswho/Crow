#include "Input.h"
#include "../Application.h"
#include <memory>

namespace Crow {
	
	short int Input::s_Keys[];
	short int Input::s_MouseKeys[];
	std::tuple<int, int> Input::s_MousePos;

	void Input::Init()
	{
		memset(s_Keys, 0, sizeof(s_Keys));
	}
	
	void Input::KeyCallback(std::tuple<int, int> key)
	{
		auto[keyCode, action] = key;
		s_Keys[keyCode] = action;
		Application::OnEvent(KeyEvent(key));
	}

	void Input::MouseCallback(std::tuple<int, int> key)
	{
		auto[keyCode, action] = key;
		s_MouseKeys[keyCode] = action;
		Application::OnEvent(MouseEvent(key));
	}

	void Input::MousePosCallback(std::tuple<int, int> pos)
	{
		s_MousePos = pos;
		Application::OnEvent(MousePosEvent(pos));
	}
}