#pragma once
#include "SDL/include/SDL.h"
#pragma comment(lib, "SDL/libx86/SDL2.lib")
#pragma comment (lib, "SDL/libx86/SDL2main.lib")
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

#include "SDL_Mixer/include/SDL_mixer.h"
#pragma comment(lib, "SDL_Mixer/libx86/SDL2_mixer.lib")

#include "Entity.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define MAX_KEYS 256


class Game
{
public:

	Game() {};
	~Game() {};
	bool Init();
	void Release();

	bool Input();
	bool Update();
	void Draw();


private:
	SDL_Window* window;
	SDL_Renderer* ren;
	Entity Player;

	SDL_Texture* background;
	SDL_Texture* player;

	enum KEY_STATE { KEY_IDLE, KEY_DOWN, KEY_REPEAT, KEY_UP };
	KEY_STATE keys[MAX_KEYS];
};