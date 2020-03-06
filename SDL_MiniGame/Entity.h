#pragma once
class Entity {
public:
	Entity();
	~Entity();

	void Init(int posx, int posy, int w, int h, int s);
	void GetRect(int* posx, int* posy, int* w, int* h);
	int  GetX();
	int  SetX(int posx);
	void ShutDown();
	bool IsAlive();
	void Move(int dx, int dy);
	int  GetWidth();

private:
	int x, y;
	int width, height;
	int speed;
	bool is_alive;
};