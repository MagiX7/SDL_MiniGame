#include "Entity.h"
#define WINDOW_HEIGTH 720


Entity::Entity()
{
	is_alive = false;
}
Entity::~Entity()
{
}
void Entity::Init(int posx, int posy, int w, int h, int s)
{
	x = posx;
	y = posy;
	width = w;
	height = h;
	speed = s;
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

int Entity::GetY()
{
	return y;
}

void Entity::SetPosition()
{
	if (y > WINDOW_HEIGTH)
	{
		y = 0;
	}
	else if (y < 0)
	{
		y = WINDOW_HEIGTH;
	}
}

bool Entity::Touching(int posx_1, int posy_1, int w_1, int h_1, int posx_2, int posy_2, int w_2, int h_2) {

	if (posx_2 > posx_1 && posx_2 < (posx_1 + w_1) && (posy_2 + h_2) > posy_1 && (posy_2 + h_2) < (posy_1 + h_1)) {
		return true;
	}
	else if (posx_2 > posx_1 && posx_2 < (posx_1 + w_1) && posy_2 > posy_1&& posy_2 < (posy_1 + h_1)) {
		return true;
	}
	else if (posx_1 < posx_2 && (posx_1 + w_1) > posx_2&& posy_1 < posy_2 && (posy_1 + h_1)> posy_2) {
		return true;
	}
	else {
		return false;
	}

}

bool Entity::TouchingShot(int posx_1, int posy_1, int w_1, int h_1, int posx_2, int posy_2, int w_2, int h_2)
{
	if ((posx_1 + w_1) > (posx_2) && (posx_1 + w_1) < (posx_2 + w_2) && (posy_1 + h_1) > posy_2 && (posy_1 + h_1) < (posy_2 + h_2)) {
		return true;
	}

	else if ((posx_1 + w_1) > (posx_2) && (posx_1 + w_1) < (posx_2 + w_2) && posy_1 > posy_2&& posy_1 < (posy_2 + h_2)) {
		return true;
	}
	else if ((posx_1 + w_1) > (posx_2) && (posx_1 + w_1) < (posx_2 + w_2) && posy_1 > posy_2 && (posy_1 + h_1) < (posy_2 + h_2)) {
		return true;
	}

	else {
		return false;
	}
}

void Entity::ShutDown()
{
	is_alive = false;
}
bool Entity::IsAlive()
{
	return is_alive;
}

void Entity::Move(int dx, int dy)
{
	x += dx * speed;
	y += dy * speed;
}

int Entity::GetWidth() {
	return width;
}

int Entity::SetX(int posx) {
	x = posx;
	return x;
}
