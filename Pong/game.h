#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <iostream>//デバックとかで
#include "SDL.h"
#include "ball.h"
#include "paddle.h"
#include "pong.h"
#ifndef M_PI
#define M_PI 3.1415926
#endif
#define WIN_W 680
#define WIN_H 480
#define WALL_W 12
#define PADDLE_W 12
#define PADDLE_H 60
#define PADDLE_SPEAD 150.0f
#define LEFT_PADDLE_POS 24.0f
#define RIGHT_PADDLE_POS WIN_W - 24.0f

#define BALL_RADIUS 12
#define BALL_SPEAD 300.0f
#define BALL_INIT_X WIN_W
#define BALL_INIT_Y WIN_H/2
#define BALL_INIT_VX cos(2.0/3.0*M_PI)
#define BALL_INIT_VY sin(2.0/3.0*M_PI)

#define PREDICTION_WAITTIME 0.1f
#define WARD_W 50
#define WARD_WG 5
#define WARD_HG 20
class Game {
public:
	Game();
	//ゲームを初期化する
	bool Initialize();
	//ゲームオーバーまでゲームループを実行する
	void RunLoop();
	//ゲームをシャットダウンする
	void Shutdown();
private:
	//ゲームループのためのヘルパー関数群
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	//SDLが作るウィンドウ
	SDL_Window* mWindow;
	//2D描画のためのレンダリング
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	//ゲームの続行を支持する
	bool mIsRunning;
	//Pong の詳細
	// ポン
	Pong* pong;
};