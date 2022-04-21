#include "Court.h"
#include "Game.h"


Court::Court()
{
	upperWall = {
		0,			// Top left x
		0,			// Top left y
		WIN_W,		// width
		WALL_W // height
	};
	lowerWall = {
		0,			// Top left x
		WIN_H - WALL_W,			// Top left y
		WIN_W,		// width
		WALL_W // height
	};
	int net_fragment_size = (WIN_H - (WALL_W * 2)) / 26;
	int x = WIN_W / 2 - net_fragment_size / 4;
	int y = 7;
	int step_size = 2 * net_fragment_size;
	for (int index = 0; index < 14; ++index) {
		net[index] = {
			x, y + index * step_size, net_fragment_size / 2, net_fragment_size
		};
	}
}
void Court::render(SDL_Renderer* renderer)
{
	// •Ç‚ðì¬
	SDL_RenderFillRect(renderer, &upperWall);
	// ‰º•Ç‚ðì¬
	SDL_RenderFillRect(renderer, &lowerWall);

	//’†‰›ƒlƒbƒg‚ð¶¬
	for (int index = 0; index < 14; ++index)
		SDL_RenderFillRect(renderer, &net[index]);
}