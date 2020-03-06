#pragma once
class Entity {
public:
	Entity();
	~Entity();

	void Init(int posx, int posy, int w, int h, int sx, int sy);
	void GetRect(int* posx, int* posy, int* w, int* h);
	int  GetX();
	int  SetX(int posx);
	void ShutDown();
	bool IsAlive();
	void Move(int dx);
	int  GetWidth();
	void Jump(int dy);
	bool IsGrounded();
	int getSpeedY(int sy);

	bool grounded;


private:
	int x = 0, y = 0;
	int width, height;
	int speedX, speedY;
	bool is_alive;
	//bool grounded;
};