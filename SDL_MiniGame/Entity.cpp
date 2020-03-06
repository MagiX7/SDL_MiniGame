#include "Entity.h"
#include <math.h>
#define PI 3.1415926535897932384626433832795

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
	grounded = true;
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

int Entity::getSpeedY(int sy)
{
	return speedY;
}

void Entity::Jump(int dy)
{
	if (!grounded)
	{
		int h = 0;
		int g = 2;
		
		
		while (h < 2)
		{
			y -= speedY;
			speedY -= g;
			h++;
		}

		if (h == 2)
		{
			speedY = 0;
		}

		while (h > 2)
		{
			y += speedY;
			speedY += -g;
			h--;
		}
		
	}
	!grounded;
}

bool Entity::IsGrounded()
{
	return grounded;
}