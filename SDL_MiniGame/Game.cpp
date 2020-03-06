#include "Game.h"
#include <math.h>


bool Game::Init()
{
	//Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	//Create window
	window = SDL_CreateWindow("Mini Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (window == NULL)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}

	//Create a Render
	ren = SDL_CreateRenderer(window, -1, 0);
	if (ren == NULL)
	{
		SDL_Log("Unable to create the render: %s", SDL_GetError());
		return false;
	}

	//Initialize the keys
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		keys[i] = KEY_IDLE;
	}

	//Initailize variables
	Player.Init(20, WINDOW_HEIGHT >> 1, 150, 100, 3);

	return true;
}

void Game::Release() 
{
	SDL_DestroyTexture(player);
	SDL_DestroyTexture(background);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Game::Input()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT) return false;
	}

	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; i++)
	{
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	return true;
}

bool Game::Update()
{

	return true;
}

void Game::Draw()
{


	//Show everything on window
	SDL_RenderPresent(ren);
	SDL_Delay(1000);
}