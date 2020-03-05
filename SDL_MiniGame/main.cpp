#include "Game.h"

int main(int argc, char* argv[])
{
	Game game;
	if (!game.Init()) return -1;

	bool end = false;
	while (!end)
	{
		end = game.Update();
		game.Draw();
	}

	game.Release();
	return 0;

}