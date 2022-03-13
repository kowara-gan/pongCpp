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
		"Game Programming in C++(Chapter1)", // ウィンドウのタイトル
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

	// 描画色を青色に設定
	SDL_SetRenderDrawColor(
		mRenderer,
		0, 0, 0, 0 // (R,G,B,A)
	);

	// バックバッファを青色で塗りつぶす
	SDL_RenderClear(mRenderer);

	// 描画色を白色に設定
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	// 壁を作成
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		WIN_W,		// width
		WALL_W // height
	};
	SDL_RenderFillRect(mRenderer, &wall);

	// 下壁を作成
	wall.y = WIN_H - WALL_W;
	SDL_RenderFillRect(mRenderer, &wall);

	//中央ネットを生成
	SDL_Rect net[14];

	int net_fragment_size = (WIN_H - (WALL_W *2)) / 26;
	int x = WIN_W / 2 - net_fragment_size / 4;
	int y =7;
	int step_size = 2 * net_fragment_size;

	for (int index = 0; index < 14; ++index) {
		net[index] = {
			x, y + index * step_size, net_fragment_size/2, net_fragment_size
		};
	}
	for (int index = 0; index < 14; ++index)
		SDL_RenderFillRect(mRenderer, &net[index]);

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