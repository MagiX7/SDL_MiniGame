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
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {

		SDL_Log("Unable to initialize: %s", SDL_GetError());
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
	Player.Init(20, WINDOW_HEIGHT >> 1, 50, 50, 5); // estaba en 50,20
	
	Menu.Init(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	//Init background image
	img_background = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Background.png"));
	int w;
	SDL_QueryTexture(img_background, NULL, NULL, &w, NULL);
	Background.Init(0, 0, w, WINDOW_HEIGHT, 4);

	idx_shot = 0;
	idx_enemies = 0;
	Music = Mix_LoadMUS("Oushit.wav");
	NukeSound = Mix_LoadWAV("Nuke.wav");
	ShotSound = Mix_LoadWAV("Laser_Gun_Sound_Effect.wav");
	

	srand(time(NULL));
	contador = 0;
	nuke = 0;
	boosterActive = false;

	//Init player image
	IMG_Init(IMG_INIT_PNG);
	img_player = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Star Fighter sprite.png"));
	img_shot = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Shot.png"));
	//Init pigeons image
	
	enemy_sprite[0] = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Pigeon Sprite 0.png"));
	enemy_sprite[1] = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Pigeon Sprite 1.png"));
	enemy_sprite[2] = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Pigeon Sprite 2.png"));
	enemy_sprite[3] = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Pigeon Sprite 3.png"));
	enemy_sprite[4] = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Pigeon Sprite 4.png"));
	enemy_sprite[5] = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Pigeon Sprite 5.png"));
	enemy_sprite[6] = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Pigeon Sprite 6.png"));
	
	//Init booster image
	img_booster = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Nuke.png"));
	
	//Init menu image
	img_menu = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Menu.png"));

	bird = 0;
	menu = true;
	return true;
}
void Game::Release()
{
	//Clean up all SDL initialized subsystems
	for (int i = 0; i < 7; i++)
	{
		SDL_DestroyTexture(enemy_sprite[i]);
	}
	SDL_DestroyTexture(img_background);
	SDL_DestroyTexture(img_player);
	SDL_DestroyTexture(img_booster);
	SDL_DestroyTexture(img_menu);
	SDL_DestroyTexture(img_shot);
	Mix_FreeMusic(Music);
	Mix_FreeChunk(NukeSound);
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

	if (menu == true) {

		for (int i = 0; i < MAX_SHOTS; ++i) {
			Shots[i].ShutDown();
		}

		if (keys[SDL_SCANCODE_X] == KEY_DOWN) {
			menu = false;
			Mix_PlayMusic(Music, -1);
		}

		if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)
		{
			Release();
			return true;
		}

	}
	else {

	

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
			Shots[idx_shot].Init(x + w - 10, y + (h >> 1) - 5, 30, 20, 10);
			idx_shot++;
			idx_shot %= MAX_SHOTS;
			Mix_PlayChannel(2, ShotSound, 0);
			Mix_VolumeChunk(ShotSound, 30);
		}
	
		//Player update

		Player.Move(fx, fy);
		Player.SetPosition();


		timeGameplay = contador/100;
		//cout << timeGameplay << endl;

		if (timeGameplay < 8) {

			difficulty = 70;

		}
		else if (timeGameplay > 8 && timeGameplay < 10)
		{
			difficulty = 10;
		}
		else if (difficulty == 2)
		{
			difficulty = 2;
		}
		else if (timeGameplay % 2 == 0) 
		{
			if (pretimeGameplay != timeGameplay)
			{
				difficulty -= 2;
			}
		}
		cout << difficulty << endl;
		cout << timeGameplay << endl;
		pretimeGameplay = timeGameplay;

		contador++;

		if (contador % difficulty == 0) //Dividimos los frames entre 5 y si el residuo nos da 0 es que es multiplo de 5, de esta manera esto ocurre cada 5 s.
		{
			int x, y, w, h;
			int pos_x = WINDOW_WIDTH - 20;
			int pos_y = rand() % WINDOW_HEIGHT;
			
			if (timeGameplay > 18)
			{
				Enemies[idx_enemies].Init(pos_x, pos_y, 30, 30, 2);
			}
			else
			{
				Enemies[idx_enemies].Init(pos_x, pos_y, 30, 30, 1);
			}
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
				Enemies[i].Move(-10, 0);
				if (Enemies[i].GetX() > WINDOW_WIDTH)  Enemies[i].ShutDown();
				if (Enemies[i].Touching(x, y, w, h, x_2, y_2, w_2, h_2) == true) {
					Mix_HaltMusic();
					SDL_Delay(600);
					//Release();
					for (int i = 0; i < AMOUNT_OF_ENEMIES; ++i)
					{
						Enemies[i].ShutDown();
						
						menu = true;
						Player.Init(20, WINDOW_HEIGHT >> 1, 50, 50, 5);
						contador = 0;
						
	
					}
					//return true;
				}
			}
		}
	
		
		//We generate the nuke!
		if (boosterActive == false) { 
			nuke = rand() % 10;
		}
		if (nuke == 2) {
			int x = rand() % (WINDOW_WIDTH/2) + 1;
			int y = rand() % WINDOW_HEIGHT - 5;
			boosterActive = true;
			Nuke.Init(x, y, 50, 50, 0);
			nuke = 0;
		}
		
		if (boosterActive == true) {
						
			int x, y, w, h;
			int x_2, y_2, w_2, h_2;
			Player.GetRect(&x, &y, &w, &h);
			Nuke.GetRect(&x_2, &y_2, &w_2, &h_2);

			if (Player.Touching(x, y, w, h, x_2, y_2, w_2, h_2) == true) {
				for (int i = 0; i < AMOUNT_OF_ENEMIES; ++i)
				{
					Enemies[i].ShutDown();
				}
				Nuke.ShutDown();
				Mix_PlayChannel(1, NukeSound, 0);
				Mix_VolumeChunk(NukeSound, 50);
				boosterActive = false;
				nukeAlpha = true;
			}
		}
		

		//Hitboxes
		for (int i = 0; i < MAX_SHOTS; i++)
		{
			int x, y, w, h;
			int x_2, y_2, w_2, h_2;
			Shots[i].GetRect(&x, &y, &w, &h);
			for (int j = 0; j < AMOUNT_OF_ENEMIES; j++)
			{
				Enemies[j].GetRect(&x_2, &y_2, &w_2, &h_2);

				if (Shots->TouchingShot(x, y, w, h, x_2, y_2, w_2, h_2) == true) {
					Enemies[j].ShutDown();
					Shots->ShutDown();
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

		//Background scroll

		Background.Move(-1, 0);
		if (Background.GetX() <= -Background.GetWidth())
		{
			Background.SetX(0);
		}
		
	}


	return false;
}

void Game::Draw()
{
	SDL_Rect rc;
	//Set the color used for drawing operations
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);

	//Clear rendering target
	SDL_RenderClear(Renderer);
	
	//Draw background scroll
	Background.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(Renderer, img_background, NULL, &rc);
	rc.x += rc.w;
	SDL_RenderCopy(Renderer, img_background, NULL, &rc);

	//Draw player
	Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_SetRenderDrawColor(Renderer, 0, 192, 0, 255);
	SDL_RenderCopy(Renderer, img_player, NULL, &rc);
	
	//Draw menu
	if (menu == true) {
		Menu.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, img_menu, NULL, &rc);
	}
	
	//Draw enemies
	for (int i = 0; i < AMOUNT_OF_ENEMIES; i++)
	{
		if (Enemies[i].IsAlive())
		{
			Enemies[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer, enemy_sprite[bird], NULL, &rc);
			if (contador % 10 == 0)
			{
				bird++;
			}
			if (bird == 6)
			{
				bird = 0;
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
			SDL_RenderCopy(Renderer, img_shot, NULL, &rc);
		}
	}

	//Draw Nuke
	if (Nuke.IsAlive() && menu == false) {

		Nuke.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, img_booster, NULL, &rc);
	}

	
	//Update screen
	SDL_RenderPresent(Renderer);

	SDL_Delay(10);	// 1000/10 = 100 fps max
}