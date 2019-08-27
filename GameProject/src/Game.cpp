#include "Crow.h"

class Layer2D : public Crow::Layer {
public:

	Layer2D()
	{}

	~Layer2D()
	{}

	void Layer2D::OnEvent(Crow::Event& e) override
	{
		auto [key, action] = e.getKeyValue();
		switch (e.m_Type)
		{
		case Crow::KEY:
			CR_GAME_INFO("Keyboard: {}", key);
			if (key == CROW_KEY_ESCAPE && action == CROW_KEY_PRESS)
				Crow::Application::Shutdown();
			break;
		case Crow::MOUSE:
			CR_GAME_INFO("Mouse Button: {}", key);
			break;
		case Crow::MOUSEPOS: // key = x, action = y
			CR_GAME_INFO("Mouse Pos: {}, {}", key, action);
			break;
		}
	}

	void Layer2D::OnUpdate() override
	{

	}
};

class Game : public Crow::Application {
	
public:
	Game()
	{
		PushLayer(new Layer2D());
	}

	~Game()
	{}

};

void main()
{
	Game* game = new Game();
	game->Run();
}