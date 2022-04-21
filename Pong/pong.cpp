#include "Pong.h"
#include "Game.h"

Pong::Pong()
{
	leftScore = 0;
	rightScore = 0;
	scene = 0;
	Ball ball0_info = Ball();
	mBalls.push_back(ball0_info);

	leftPaddle = new Paddle(LEFT_PADDLE_POS, WIN_H / 2.0f, false);
	rightPaddle = new Paddle(RIGHT_PADDLE_POS, WIN_H / 2.0f, false);

	leftScoreAlphaNum = Alphanum::getAlphanum(WIN_W * 0.25f - WARD_W / 2, 30, 0);
	rightScoreAlphaNum = Alphanum::getAlphanum(WIN_W * 0.75f - WARD_W / 2, 30, 0);
	sinceScoreUp = 0;
	eraseBall.isDef=false;
	court = Court();
}
Pong::~Pong()
{
	delete leftPaddle;
	delete rightPaddle;
}
void Pong::init()
{
	//起動直後の状態に戻す
	leftScore = 0;
	rightScore = 0;
	scene = 0;
	for (int i = 0; i < mBalls.size() - 1; i++)
		mBalls.pop_back();
	mBalls[0].init();
	leftScoreAlphaNum = Alphanum::getAlphanum(WIN_W * 0.25f - WARD_W / 2, 30, 0);
	rightScoreAlphaNum = Alphanum::getAlphanum(WIN_W * 0.75f - WARD_W / 2, 30, 0);
	rightPaddle->setAI(false);
	leftPaddle->setAI(false);
	sinceScoreUp = 0;
}
void Pong::update(float deltaTime)
{
	//データ更新（全てのシーン）
	rightPaddle->update(deltaTime, mBalls);
	leftPaddle->update(deltaTime, mBalls);
	
	//データ更新（シーン１）
	if (scene == 1) {
		for (auto it = mBalls.begin();it!=mBalls.end();it++)
			it->update(leftPaddle, rightPaddle, this, deltaTime, it);
		if (sinceScoreUp < 20) {
			sinceScoreUp += deltaTime;
		}
		else {
			Ball ball = Ball();
			mBalls.push_back(ball);
			sinceScoreUp = 0;
		}
		if (eraseBall.isDef) {
			for (auto& it : eraseBall.its) {
				if(mBalls.size()>=2)mBalls.erase(it);
			}
			eraseBall.its.clear();
			eraseBall.isDef = false;
		}
		return;
	}
}
void Pong::leftScoreUp(std::vector<Ball>::iterator it) {
	//左の点追加
	leftScore += 1;
	leftScoreAlphaNum = Alphanum::getAlphanum(WIN_W * 0.25f - WARD_W / 2, 30, leftScore);
	if (leftScore == 10)scene = 2;
	sinceScoreUp = 0;
	if (mBalls.size() >= 2) {
		eraseBall.its.push_back(it);
		eraseBall.isDef = true;
	}
}
void Pong::rightScoreUp(std::vector<Ball>::iterator it) {
	//右の点追加
	rightScore += 1;
	rightScoreAlphaNum = Alphanum::getAlphanum(WIN_W * 0.75f - WARD_W / 2, 30, rightScore);
	if (rightScore == 10)scene = 2;
	sinceScoreUp = 0;
	if (mBalls.size() >= 1) {
		eraseBall.its.push_back(it);
		eraseBall.isDef = true;
	}
}

void Pong::input(const Uint8* state) {
	//入力情報に基づいて処理
	//左パドル操作
	if (!leftPaddle->isAI)leftPaddle->moveStop();
	if (state[SDL_SCANCODE_W]) {
		leftPaddle->moveUp();
	}
	if (state[SDL_SCANCODE_S]) {
		leftPaddle->moveDown();
	}
	//右パドル操作
	if (!rightPaddle->isAI)rightPaddle->moveStop();
	if (state[SDL_SCANCODE_I]) {
		rightPaddle->moveUp();
	}
	if (state[SDL_SCANCODE_K]) {
		rightPaddle->moveDown();
	}
	//リトライ操作
	if (state[SDL_SCANCODE_R]) {
		init();
	}
	//スタート操作（シーン0）
	if (scene == 0) {
		if (state[SDL_SCANCODE_1]) {
			scene = 1;
			rightPaddle->setAI(true);
		}
		if (state[SDL_SCANCODE_2]) {
			if (scene == 0)scene = 1;

		}
		if (state[SDL_SCANCODE_0]) {
			if (scene == 0)scene = 1;
			rightPaddle->setAI(true);
			leftPaddle->setAI(true);
		}
		return;
	}
}
void Pong::render(SDL_Renderer* renderer)
{
	//描画処理
	rightPaddle->render(renderer);
	leftPaddle->render(renderer);
	for (auto& ball : mBalls)
		ball.render(renderer);
	court.render(renderer);
	if (scene == 0) {
		Alphanum::writeAlphanum(renderer, 40, 30, 19);//upArrow
		Alphanum::writeAlphanum(renderer, 40 + WARD_W + WARD_WG, 30, 10);//w
		Alphanum::writeAlphanum(renderer, 40, WIN_H-30- WARD_W, 20);//downArrow
		Alphanum::writeAlphanum(renderer, 40 + WARD_W + WARD_WG, WIN_H - 30 - WARD_W, 11);//s
		Alphanum::writeAlphanum(renderer, WIN_W - 40 - WARD_W * 2 - WARD_WG, 30, 19);//upArrow
		Alphanum::writeAlphanum(renderer, WIN_W - 40 - WARD_W * 1 , 30, 21);//i
		Alphanum::writeAlphanum(renderer, WIN_W - 40 - WARD_W * 2 - WARD_WG, WIN_H - 30 - WARD_W, 20);//downArrow
		Alphanum::writeAlphanum(renderer, WIN_W - 40 - WARD_W * 1, WIN_H - 30 - WARD_W, 22);//k

		Alphanum::writeAlphanum(renderer, WIN_W / 4 - WARD_W * 2.5f - WARD_WG * 2, WIN_H / 2 - WARD_HG * 1.5f - WARD_W * 2, 11);//s
		Alphanum::writeAlphanum(renderer, WIN_W / 4 - WARD_W * 1.5f - WARD_WG, WIN_H / 2 - WARD_HG * 1.5f - WARD_W * 2, 17);//t
		Alphanum::writeAlphanum(renderer, WIN_W / 4 - WARD_W * 0.5f, WIN_H / 2 - WARD_HG * 1.5f - WARD_W * 2, 15);//a
		Alphanum::writeAlphanum(renderer, WIN_W / 4 + WARD_W * 0.5f + WARD_WG, WIN_H / 2 - WARD_HG * 1.5f - WARD_W * 2, 16);//r
		Alphanum::writeAlphanum(renderer, WIN_W / 4 + WARD_W * 1.5f + WARD_WG * 2, WIN_H / 2 - WARD_HG * 1.5f - WARD_W * 2, 17);//t
		Alphanum::writeAlphanum(renderer, WIN_W * 0.75f - WARD_W * 2.5f - WARD_WG * 2, WIN_H / 2 - WARD_HG * 1.5f - WARD_W * 2, 1);//1
		Alphanum::writeAlphanum(renderer, WIN_W * 0.75f - WARD_W * 1.5f - WARD_WG, WIN_H / 2 - WARD_HG * 1.5f - WARD_W * 2, 12);//p
		Alphanum::writeAlphanum(renderer, WIN_W * 0.75f + WARD_W * 0.5f + WARD_WG, WIN_H / 2 - WARD_HG * 1.5f - WARD_W * 2, 1);//1
		Alphanum::writeAlphanum(renderer, WIN_W * 0.75f - WARD_W * 2.5f - WARD_WG * 2, WIN_H / 2 - WARD_HG * 0.5f - WARD_W , 2);//2
		Alphanum::writeAlphanum(renderer, WIN_W * 0.75f - WARD_W * 1.5f - WARD_WG, WIN_H / 2 - WARD_HG * 0.5f - WARD_W , 12);//p
		Alphanum::writeAlphanum(renderer, WIN_W * 0.75f + WARD_W * 0.5f + WARD_WG, WIN_H / 2 - WARD_HG * 0.5f - WARD_W , 2);//2
		Alphanum::writeAlphanum(renderer, WIN_W * 0.75f - WARD_W * 2.5f - WARD_WG * 2, WIN_H / 2 + WARD_HG * 0.5f , 0);//0
		Alphanum::writeAlphanum(renderer, WIN_W * 0.75f - WARD_W * 1.5f - WARD_WG, WIN_H / 2 + WARD_HG * 0.5f , 12);//p
		Alphanum::writeAlphanum(renderer, WIN_W * 0.75f + WARD_W * 0.5f + WARD_WG, WIN_H / 2 + WARD_HG * 0.5f , 0);//0
		Alphanum::writeAlphanum(renderer, WIN_W / 4 - WARD_W * 2.5f - WARD_WG * 2, WIN_H / 2 + WARD_HG * 1.5f + WARD_W, 16);//r
		Alphanum::writeAlphanum(renderer, WIN_W / 4 - WARD_W * 1.5f - WARD_WG, WIN_H / 2 + WARD_HG * 1.5f + WARD_W, 14);//e
		Alphanum::writeAlphanum(renderer, WIN_W / 4 - WARD_W * 0.5f, WIN_H / 2 + WARD_HG * 1.5f + WARD_W, 17);//t
		Alphanum::writeAlphanum(renderer, WIN_W / 4 + WARD_W * 0.5f + WARD_WG, WIN_H / 2 + WARD_HG * 1.5f + WARD_W, 16);//r
		Alphanum::writeAlphanum(renderer, WIN_W / 4 + WARD_W * 1.5f + WARD_WG * 2, WIN_H / 2 + WARD_HG * 1.5f + WARD_W, 18);//y
		Alphanum::writeAlphanum(renderer, WIN_W * 0.75f - WARD_W * 0.5f, WIN_H / 2 + WARD_HG * 1.5f + WARD_W, 16);//r
		return;
	}
	if (scene == 1){
		Alphanum::write(renderer, leftScoreAlphaNum);
		Alphanum::write(renderer, rightScoreAlphaNum);
		return;
	}
	if (scene == 2) {
		Alphanum::write(renderer, leftScoreAlphaNum);
		Alphanum::write(renderer, rightScoreAlphaNum);
		Alphanum::writeAlphanum(renderer, WIN_W / 4 - WARD_W * 2.5f - WARD_WG * 2, WIN_H / 2 + WARD_HG * 1.5f + WARD_W, 16);//r
		Alphanum::writeAlphanum(renderer, WIN_W / 4 - WARD_W * 1.5f - WARD_WG, WIN_H / 2 + WARD_HG * 1.5f + WARD_W, 14);//e
		Alphanum::writeAlphanum(renderer, WIN_W / 4 - WARD_W * 0.5f, WIN_H / 2 + WARD_HG * 1.5f + WARD_W, 17);//t
		Alphanum::writeAlphanum(renderer, WIN_W / 4 + WARD_W * 0.5f + WARD_WG, WIN_H / 2 + WARD_HG * 1.5f + WARD_W, 16);//r
		Alphanum::writeAlphanum(renderer, WIN_W / 4 + WARD_W * 1.5f + WARD_WG * 2, WIN_H / 2 + WARD_HG * 1.5f + WARD_W, 18);//y
		Alphanum::writeAlphanum(renderer, WIN_W * 0.75f - WARD_W * 0.5f, WIN_H / 2 + WARD_HG * 1.5f + WARD_W, 16);//r
		return;
	}
}
