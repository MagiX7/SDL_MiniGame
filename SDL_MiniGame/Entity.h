#pragma once
class Entity
{
public:
	Entity();
	~Entity();

	void Init(int posx, int posy, int w, int h, int s);
	void GetRect(int* posx, int* posy, int* w, int* h);
	int  GetX();
	bool Touching(int posx_1, int posy_1, int w_1, int h_1, int posx_2, int posy_2, int w_2, int h_2);
	void ShutDown();
	bool IsAlive();
	void Move(int dx, int dy);

private:
	int x, y;
	int width, height;
	int speed;
	bool is_alive;
};