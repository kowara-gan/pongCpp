#include "Paddle.h"
#include "Game.h"

Paddle::Paddle(float x, float y,bool isAI)
{
	prediction = {0,0,false};
	pos_x = x;
	pos_y = y;
	dil_y = 0;
	this->isAI = isAI;
}
void Paddle::setAI(bool flg)
{
	isAI = flg;
}
float Paddle::getPosX()
{
	return pos_x;
}
float Paddle::getPosY()
{
	return pos_y;
}
void Paddle::moveUp()
{
	dil_y = -1;
}
void Paddle::moveDown()
{
	dil_y = 1;
}
void Paddle::moveStop()
{
	dil_y = 0;
}
void Paddle::render(SDL_Renderer* renderer)
{
	// パドル作成
	SDL_Rect paddle0{
		static_cast<int>(this->pos_x- PADDLE_W /2.0f),
		static_cast<int>(this->pos_y - PADDLE_H / 2.0f),
		PADDLE_W,
		static_cast<int>(PADDLE_H)
	};
	SDL_RenderFillRect(renderer, &paddle0);
}
void Paddle::update(float deltaTime, vector<Ball> balls)
{
	if (isAI) {
		//aiで操作する
		Ball targetBall=balls[0];
		if (pos_x < WIN_W / 2.0f) {
			for (auto& ball:balls) {
				if (ball.vel_x > 0)continue;
				if (targetBall.pos_x > ball.pos_x) {
					targetBall = ball;
				}
				else if (targetBall.vel_x > 0) {
					targetBall = ball;
				}
			}
		}
		else {
			for (auto& ball : balls) {
				if (ball.vel_x < 0)continue;
				if (targetBall.pos_x < ball.pos_x) {
					targetBall = ball;
				}
				else if (targetBall.vel_x < 0) {
					targetBall = ball;
				}
			}
		}
		this->ai(deltaTime,&targetBall);
	}
	if (dil_y != 0)
	{
		pos_y += dil_y * PADDLE_SPEAD * deltaTime;
		// パドルが画面から出ないようにする
		if (pos_y < (PADDLE_H / 2.0f + WALL_W))
		{
			pos_y = PADDLE_H / 2.0f + WALL_W;
		}
		else if (pos_y > (WIN_H - PADDLE_H / 2.0f - WALL_W))
		{
			pos_y = WIN_H - PADDLE_H / 2.0f - WALL_W;
		}
	}
}
void Paddle::ai(float dt, Ball* ball) {
	//ボールが離れるとき、パドルは動かない（真ん中に戻るのもあり）
	if (ball->vel_x < 0 && pos_x> WIN_W / 2.0f || ball->vel_x > 0 && pos_x < WIN_W / 2.0f){
		this->dil_y = 0;
		return;
	}
	//予想設定
	this->predict(dt,ball);
	//予想がある場合
	if (prediction.isDef) {
		//予想に基づいて操作
		if (this->prediction.y < this->pos_y - PADDLE_H / 2.0f)this->dil_y = -1;
		else if (prediction.y > this->pos_y + PADDLE_H / 2.0f)this->dil_y = 1;
		else this->dil_y = 0;
	}
}
void Paddle::predict(float dt, Ball* ball) {
	//予想するタイミングを図る
	if (prediction.since < PREDICTION_WAITTIME) {
		//待機デルタタイムに満たないので予想しない
		prediction.since += dt;
		prediction.isDef = false;
		return;
	}
	prediction.isDef = true;
	float gapX = (pos_x > WIN_W / 2.0f) ? PADDLE_W / 2.0f : -PADDLE_W / 2.0f;
	//衝突位置予想
	InterceptPoint ip = ball->intercept(ball->pos_x, ball->pos_y, ball->pos_x + ball->vel_x * 10000.0f ,
		ball->pos_y + ball->vel_y * 10000.0f , this->pos_x - gapX, 10000.0f, this->pos_x - gapX, -10000.0f);
	//折り返しで予想値を範囲内にする
	while (ip.y < WALL_W || ip.y>WIN_H- WALL_W) {
		if (ip.y < WALL_W ) {
			ip.y = WALL_W-(ip.y-WALL_W);
		}
		else if (ip.y > WIN_H - WALL_W) {
			ip.y = WIN_H-WALL_W-(ip.y-(WIN_H-WALL_W));
		}
	}
	this->prediction.y = ip.y;
	prediction.since = 0;
}

