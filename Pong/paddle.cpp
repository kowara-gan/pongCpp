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
	// �p�h���쐬
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
		//ai�ő��삷��
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
		// �p�h������ʂ���o�Ȃ��悤�ɂ���
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
	//�{�[���������Ƃ��A�p�h���͓����Ȃ��i�^�񒆂ɖ߂�̂�����j
	if (ball->vel_x < 0 && pos_x> WIN_W / 2.0f || ball->vel_x > 0 && pos_x < WIN_W / 2.0f){
		this->dil_y = 0;
		return;
	}
	//�\�z�ݒ�
	this->predict(dt,ball);
	//�\�z������ꍇ
	if (prediction.isDef) {
		//�\�z�Ɋ�Â��đ���
		if (this->prediction.y < this->pos_y - PADDLE_H / 2.0f)this->dil_y = -1;
		else if (prediction.y > this->pos_y + PADDLE_H / 2.0f)this->dil_y = 1;
		else this->dil_y = 0;
	}
}
void Paddle::predict(float dt, Ball* ball) {
	//�\�z����^�C�~���O��}��
	if (prediction.since < PREDICTION_WAITTIME) {
		//�ҋ@�f���^�^�C���ɖ����Ȃ��̂ŗ\�z���Ȃ�
		prediction.since += dt;
		prediction.isDef = false;
		return;
	}
	prediction.isDef = true;
	float gapX = (pos_x > WIN_W / 2.0f) ? PADDLE_W / 2.0f : -PADDLE_W / 2.0f;
	//�Փˈʒu�\�z
	InterceptPoint ip = ball->intercept(ball->pos_x, ball->pos_y, ball->pos_x + ball->vel_x * 10000.0f ,
		ball->pos_y + ball->vel_y * 10000.0f , this->pos_x - gapX, 10000.0f, this->pos_x - gapX, -10000.0f);
	//�܂�Ԃ��ŗ\�z�l��͈͓��ɂ���
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

