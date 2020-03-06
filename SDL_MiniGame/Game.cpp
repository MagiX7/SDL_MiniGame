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

	//Initialize variables
	Player.Init(20, WINDOW_HEIGHT >> 1, 70, 50, 3, 0); //Initialize Player position, and size.
	Water.Init(0, WINDOW_HEIGHT - 40, WINDOW_WIDTH, 40, 0, 0); //Initialize water position, and size.
	Brick.Init(720, WINDOW_HEIGHT - 200, 300, 33, 0, 0); //Initialize brick position, and size.

	//Initialize Textures
	IMG_Init(IMG_INIT_PNG);
	player_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("Player.png"));
	water_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("Water.png"));
	brick_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("brickStack.png"));

	return true;
}

void Game::Release() 
{
	SDL_DestroyTexture(water_img);
	SDL_DestroyTexture(brick_img);
	SDL_DestroyTexture(player_img);
	SDL_DestroyTexture(background);

	SDL_DestroyWindow(window);

	SDL_Quit();
}


bool Game::Input()
{
	
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)	return false;
	}

	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
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
	if (!Input()) return true;
	//Process Input
	int fx = 0, fy = 0;
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_A] == KEY_REPEAT)	fx = -1;
	if (keys[SDL_SCANCODE_D] == KEY_REPEAT)	fx = 1;
	if (keys[SDL_SCANCODE_SPACE] == KEY_DOWN)
	{
		Player.grounded = false;
	}
	
	Player.Jump(fy);
	Player.Move(fx);
	
	return false;
}

void Game::Draw()
{
	SDL_Rect rc;

	//Print background black color
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	
	//Print player texture
	Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(renderer, player_img, NULL, &rc);
	


	//Print Bricks texture
	Brick.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(renderer, brick_img, NULL, &rc);
	

	//Print rectangle foreground for the water below
	Water.GetRect(&rc.x,&rc.y,&rc.w,&rc.h); //We get the rectangle stadistics for the water
	SDL_SetRenderDrawColor(renderer, 97, 170, 255, 255);
	//SDL_RenderFillRect(renderer, &rc);
	SDL_RenderCopy(renderer, water_img, NULL, &rc);
	
	//Show everything on window
	SDL_RenderPresent(renderer);
	SDL_Delay(10);
}