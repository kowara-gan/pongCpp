#include "game.h"

Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mTicksCount = 0;
	mIsRunning = true;
	pong = new Pong();
}

bool Game::Initialize() {
	// SDL初期化
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("SDLを初期化できません : %s", SDL_GetError());
		return false;
	}

	// SDLウィンドウを作成
	mWindow = SDL_CreateWindow(
		"Pong in C++", // ウィンドウのタイトル
		100, // ウィンドウ左上隅のx座標
		100, // ウィンドウ左上隅のy座標
		WIN_W, // ウィンドウの幅(width)
		WIN_H, // ウィンドウの高さ(height)
		0 // フラグ(設定しない時は0)
	);

	if (!mWindow) {
		SDL_Log("ウィンドウの作成に失敗しました : %s", SDL_GetError());
		return false;
	}

	// SDLレンダラーを作成
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	return true;
}

void Game::ProcessInput() {
	SDL_Event event;
	// キューにイベントがあれば繰り返す
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);
	//escでゲーム終了
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}
	pong->input(state);
}

void Game::UpdateGame() {
	//前のフレームから16msが経過するまで待つ
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// deltatimeは前のフレームとの時刻の差を秒に変換した値
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	//deltatimeを最大値で制限する
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
	//パドル用
	mTicksCount = SDL_GetTicks();
	pong->update(deltaTime);
}

void Game::GenerateOutput() {
	// 背景
	// 描画色を青色に設定
	SDL_SetRenderDrawColor(
		mRenderer,
		0, 0, 0, 0 // (R,G,B,A)
	);
	// バックバッファを黒色で塗りつぶす
	SDL_RenderClear(mRenderer);

	// 描画色を白色に設定
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	pong->render(mRenderer);
	// バックバッファとフロントバッファを入れ替え
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	delete pong;
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}