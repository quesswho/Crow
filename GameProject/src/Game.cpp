#include "Crow.h"
#include <iostream>

class Game : public Crow::Application {

	Game::~Game()
	{}

};

int main()
{
	Game* game = new Game();
	game->Run();
	return 0;
}