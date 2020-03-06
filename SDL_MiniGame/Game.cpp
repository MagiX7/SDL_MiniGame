#include "Game.h"
#include "Entity.h"
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
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL)
	{
		SDL_Log("Unable to create the render: %s", SDL_GetError());
		return false;
	}

	//Initialize the keys
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		keys[i] = KEY_IDLE;
	}
	//Initialize Textures
	IMG_Init(IMG_INIT_PNG);
	water_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("Water.png"));

	//Initialize variables
	Player.Init(20, WINDOW_HEIGHT >> 1, 150, 100, 3);
	Water.Init(0, WINDOW_HEIGHT - 40 , WINDOW_WIDTH, 40, 0);

	return true;
}

void Game::Release() 
{
	SDL_DestroyTexture(water_img);
	SDL_DestroyTexture(player);
	SDL_DestroyTexture(background);
	SDL_DestroyRenderer(renderer);
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
	//Update keyboard input

	return true;
}

void Game::Draw()
{
	SDL_Rect rc;

	//Print background black color
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//Print rectangle foreground for the water below
	Water.GetRect(&rc.x,&rc.y,&rc.w,&rc.h); //We get the rectangle stadistics for the water
	SDL_SetRenderDrawColor(renderer, 97, 170, 255, 255);
	//SDL_RenderFillRect(renderer, &rc);
	SDL_RenderCopy(renderer, water_img, NULL, &rc);
	
	//SDL_RenderDrawRect(renderer, &rc);
	
	


	//Show everything on window
	SDL_RenderPresent(renderer);
	SDL_Delay(5000);
}