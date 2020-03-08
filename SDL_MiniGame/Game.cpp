#include "Game.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
using namespace std;

Game::Game() {}
Game::~Game() {}

bool Game::Init()
{
	//Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	//Create our window: title, x, y, w, h, flags
	Window = SDL_CreateWindow("Spaceship: arrow keys + space", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (Window == NULL)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}
	//Create a 2D rendering context for a window: window, device index, flags
	Renderer = SDL_CreateRenderer(Window, -1, 0);
	if (Renderer == NULL)
	{
		SDL_Log("Unable to create rendering context: %s", SDL_GetError());
		return false;
	}
	//Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i)
		keys[i] = KEY_IDLE;

	//Init variables
	Player.Init(20, WINDOW_HEIGHT >> 1, 70, 70, 5); // estaba en 50,20
	idx_shot = 0;
	idx_enemies = 0;

	srand(time(NULL));
	contador = 0;

	IMG_Init(IMG_INIT_PNG);
	img_player = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Star Fighter sprite.png"));
	enemy_sprite[0] = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Pigeon Sprite 0.png"));
	enemy_sprite[1] = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Pigeon Sprite 1.png"));
	enemy_sprite[2] = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Pigeon Sprite 2.png"));
	enemy_sprite[3] = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Pigeon Sprite 3.png"));
	enemy_sprite[4] = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Pigeon Sprite 4.png"));
	enemy_sprite[5] = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Pigeon Sprite 5.png"));
	enemy_sprite[6] = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Pigeon Sprite 6.png"));
	enemy_sprite[7] = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Pigeon Sprite 7.png"));

	return true;
}
void Game::Release()
{
	//Clean up all SDL initialized subsystems
	for (int i = 0; i < 8; i++)
	{
		SDL_DestroyTexture(enemy_sprite[i]);
	}
	SDL_DestroyTexture(img_player);
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
	//Read Input
	if (!Input())	return true;

	//Process Input
	int fx = 0, fy = 0;
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_W] == KEY_REPEAT)	fy = -1;
	if (keys[SDL_SCANCODE_S] == KEY_REPEAT)	fy = 1;
	if (keys[SDL_SCANCODE_A] == KEY_REPEAT)	fx = -1;
	if (keys[SDL_SCANCODE_D] == KEY_REPEAT)	fx = 1;
	if (keys[SDL_SCANCODE_SPACE] == KEY_DOWN)
	{
		int x, y, w, h;
		Player.GetRect(&x, &y, &w, &h);
		Shots[idx_shot].Init(x + w - 10, y + (h >> 1) - 5, 20, 10, 10);
		idx_shot++;
		idx_shot %= MAX_SHOTS;
	}

	//Player update
	Player.Move(fx, fy);

	

	contador++;

	if (contador % 10 == 0) //Dividimos los frames entre 5 y si el residuo nos da 0 es que es multiplo de 5, de esta manera esto ocurre cada 5 s.
	{
		int x, y, w, h;
		int pos_x = WINDOW_WIDTH - 20;
		int pos_y = rand() % WINDOW_HEIGHT;

		Enemies[idx_enemies].Init(pos_x, pos_y, 20, 10, 2);
		Enemies[idx_enemies].GetRect(&x, &y, &w, &h);

		idx_enemies++;
		idx_enemies %= AMOUNT_OF_ENEMIES;

	}

	//Logic
	//Enemies update



	for (int i = 0; i < AMOUNT_OF_ENEMIES; ++i)
	{

		if (Enemies[i].IsAlive())
		{
			int x, y, w, h;
			int x_2, y_2, w_2, h_2;
			Player.GetRect(&x, &y, &w, &h);
			Enemies[i].GetRect(&x_2, &y_2, &w_2, &h_2);
			Enemies[i].Move(-2, 0);
			if (Enemies[i].GetX() > WINDOW_WIDTH)  Enemies[i].ShutDown();
			if (Enemies[i].Touching(x, y, w, h, x_2, y_2, w_2, h_2) == true) {

				SDL_Delay(500);
				Release();

			}
		}


	}


	//Shots update
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		
		if (Shots[i].IsAlive())
		{
			Shots[i].Move(1, 0);

			if (Shots[i].GetX() > WINDOW_WIDTH)	Shots[i].ShutDown();	
		}
	}

	//Hitboxes
	for (int i = 0; i < MAX_SHOTS; i++)
	{
		int x, y, w, h;
		int x_2,y_2, w_2, h_2;
		Shots[i].GetRect(&x, &y, &w, &h);
		for (int j = 0; j < AMOUNT_OF_ENEMIES; j++)
		{
			Enemies[j].GetRect(&x_2, &y_2, &w_2, &h_2);

			if (Shots[i].Touching(x, y, w, h, x_2, y_2, w_2, h_2) == true) {
				Enemies[j].ShutDown();
				Shots[i].ShutDown();
			}

		}
		
	}
	

	return false;
}
void Game::Draw()
{
	//Set the color used for drawing operations
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	//Clear rendering target
	SDL_RenderClear(Renderer);

	//Draw player
	SDL_Rect rc;
	Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_SetRenderDrawColor(Renderer, 0, 192, 0, 255);
	SDL_RenderCopy(Renderer, img_player, NULL, &rc);

	//Draw enemies
	SDL_SetRenderDrawColor(Renderer, 0, 255, 0, 255);
	for (int i = 0; i < AMOUNT_OF_ENEMIES; i++)
	{
		if (Enemies[i].IsAlive())
		{
			Enemies[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			for (int i = 0; i < 8; i++)
			{
				SDL_RenderCopy(Renderer, enemy_sprite[i], NULL, &rc);
			}
		}
	}

	//Draw shots
	SDL_SetRenderDrawColor(Renderer, 192, 0, 0, 255);
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Shots[i].IsAlive())
		{
			Shots[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderFillRect(Renderer, &rc);
		}
	}

	//Update screen
	SDL_RenderPresent(Renderer);

	SDL_Delay(10);	// 1000/10 = 100 fps max
}