#include "Input.h"
#include "../Application.h"
#include <memory>

namespace Crow {
	
	short int Input::s_Keys[];
	short int Input::s_MouseKeys[];
	Math::TVec2<double> Input::s_MousePos;
	bool Input::s_Focused;

	void Input::Init(double x, double y)
	{
		memset(s_Keys, 0, sizeof(s_Keys)); // Set all keys to CROW_RELEASE
		memset(s_MouseKeys, 0, sizeof(s_MouseKeys));
		s_MousePos = Math::TVec2<double>(x, y);
	}
	
	void Input::KeyCallback(Math::TVec2<int> key)
	{
		s_Keys[key.x] = key.y;
		Application::OnEvent(KeyEvent(key));
	}

	void Input::MouseCallback(Math::TVec2<int> key)
	{
		s_MouseKeys[key.x] = key.y;
		Application::OnEvent(MouseEvent(key));
	}

	void Input::MousePosCallback(Math::TVec2<double> pos)
	{
		s_MousePos = pos;
		Application::OnEvent(MousePosEvent(Math::TVec2<int>(pos.x, pos.y)));
	}

	void Input::MouseChange(int mx, int my)
	{
		MousePosCallback(Math::TVec2<double>(s_MousePos.x + mx, s_MousePos.y + my));
	}

	void Input::WindowFocusCallback(bool focused)
	{
		s_Focused = focused;
	}
}