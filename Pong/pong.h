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
//�{�[���폜�̂��߂̍\����
struct EraseBall {
	vector<vector<Ball>::iterator> its;
	bool isDef;
};

class Pong
{
private:
	//0���X�^���o�C�A1���v���C�A2�����U���g
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
	//�o�h��
	Paddle* leftPaddle;
	Paddle* rightPaddle;
	//�{�[��
	vector<Ball> mBalls;
};