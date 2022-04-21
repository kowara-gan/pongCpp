#pragma once
#include "SDL.h"
#include "Paddle.h"
#include "Pong.h"
#include <vector>
class Pong;
class Paddle;

//�Փˈʒu�̂��߂̍\����
struct InterceptPoint {
	float x;
	float y;
	int side;//0�͌������Ȃ��A1�͍��A2�͉E�A3�͏�A4�͉��̌���
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