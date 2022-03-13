#pragma once
#include "SDL.h"
#include "ball.h"
#include <vector>
class Ball;
//衝突予想に使用する構造体
struct PredictPoint {
	float y;
	float since;
	bool isDef;
};
class Paddle
{
private:
	PredictPoint prediction;
	float pos_x, pos_y;
	float dil_y;
public:
	bool isAI;
	Paddle(float x1, float y1, bool isAI);
	void setAI(bool flg);
	float getPosX();
	float getPosY();
	void moveUp();
	void moveDown();
	void moveStop();
	void render(SDL_Renderer*);
	void update(float, std::vector<Ball>);
	void ai(float,Ball*);
	void predict(float, Ball*);
};