#pragma once
#include "SDL.h"
#include "Paddle.h"
#include "Pong.h"
#include <vector>
class Pong;
class Paddle;

//衝突位置のための構造体
struct InterceptPoint {
	float x;
	float y;
	int side;//0は交差しない、1は左、2は右、3は上、4は下の交差
};

class Ball
{
public:
	float pos_x, pos_y;
	float vel_x, vel_y;
	Ball();
	void init();
	void update(Paddle*, Paddle*,Pong*,float,std::vector<Ball>::iterator);
	InterceptPoint intercept(float,float,float,float,float,float,float,float,int side = 5);
	void render(SDL_Renderer*);
};