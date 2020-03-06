#include "Entity.h"


Entity::Entity()
{
	is_alive = false;
}

Entity::~Entity() 
{
}

void Entity::Init(int posx, int posy, int w, int h, int sx, int sy) 
{
	x = posx;
	y = posy;
	width = w;
	height = h;
	speedX = sx;
	is_alive = true;
}

void Entity::GetRect(int* posx, int* posy, int* w, int* h) 
{
	*posx = x;
	*posy = y;
	*w = width;
	*h = height;
}

int Entity::GetX()
{
	return x;
}

int Entity::SetX(int posx) 
{
	x = posx;
	return x;
}

void Entity::ShutDown() 
{
	is_alive = false;
}

bool Entity::IsAlive()
{
	return is_alive;
}

void Entity::Move(int dx) 
{
	x += dx * speedX;
}

int Entity::GetWidth()
{
	return width;
}

void Entity::Jump(int dy)
{
	y -= dy*speedY;
}