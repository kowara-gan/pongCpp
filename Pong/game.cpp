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
	// SDL������
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("SDL���������ł��܂��� : %s", SDL_GetError());
		return false;
	}

	// SDL�E�B���h�E���쐬
	mWindow = SDL_CreateWindow(
		"Game Programming in C++(Chapter1)", // �E�B���h�E�̃^�C�g��
		100, // �E�B���h�E�������x���W
		100, // �E�B���h�E�������y���W
		WIN_W, // �E�B���h�E�̕�(width)
		WIN_H, // �E�B���h�E�̍���(height)
		0 // �t���O(�ݒ肵�Ȃ�����0)
	);

	if (!mWindow) {
		SDL_Log("�E�B���h�E�̍쐬�Ɏ��s���܂��� : %s", SDL_GetError());
		return false;
	}

	// SDL�����_���[���쐬
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	return true;
}

void Game::ProcessInput() {
	SDL_Event event;
	// �L���[�ɃC�x���g������ΌJ��Ԃ�
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
	//esc�ŃQ�[���I��
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}
	pong->input(state);
}

void Game::UpdateGame() {
	//�O�̃t���[������16ms���o�߂���܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// deltatime�͑O�̃t���[���Ƃ̎����̍���b�ɕϊ������l
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	//deltatime���ő�l�Ő�������
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
	//�p�h���p
	mTicksCount = SDL_GetTicks();
	pong->update(deltaTime);
}

void Game::GenerateOutput() {

	// �`��F��F�ɐݒ�
	SDL_SetRenderDrawColor(
		mRenderer,
		0, 0, 0, 0 // (R,G,B,A)
	);

	// �o�b�N�o�b�t�@��F�œh��Ԃ�
	SDL_RenderClear(mRenderer);

	// �`��F�𔒐F�ɐݒ�
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	// �ǂ��쐬
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		WIN_W,		// width
		WALL_W // height
	};
	SDL_RenderFillRect(mRenderer, &wall);

	// ���ǂ��쐬
	wall.y = WIN_H - WALL_W;
	SDL_RenderFillRect(mRenderer, &wall);

	//�����l�b�g�𐶐�
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
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ�
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