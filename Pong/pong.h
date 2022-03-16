#pragma once
#include "SDL.h"
#include "ball.h"
#include "paddle.h"
#include "court.h"
#include "alphanum.h"
#include <vector>
using namespace std;
class Ball;
class Paddle;
//ボール削除のための構造体
struct EraseBall {
	vector<vector<Ball>::iterator> its;
	bool isDef;
};

class Pong
{
private:
	//0がスタンバイ、1がプレイ、2がリザルト
	int scene;
	int leftScore, rightScore;
	vector<SDL_Rect> leftScoreAlphaNum;
	vector<SDL_Rect> rightScoreAlphaNum;
	float sinceScoreUp;
	EraseBall eraseBall;
	Court court;
public:
	Pong();
	~Pong();
	void render(SDL_Renderer*);
	void init();
	void update(float);
	void input(const Uint8*);
	void leftScoreUp(std::vector<Ball>::iterator);
	void rightScoreUp(std::vector<Ball>::iterator);
	//バドル
	Paddle* leftPaddle;
	Paddle* rightPaddle;
	//ボール
	vector<Ball> mBalls;
};