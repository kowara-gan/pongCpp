#pragma once
#include "SDL.h"


class Court
{
private:
	SDL_Rect upperWall;
	SDL_Rect lowerWall;
	SDL_Rect net[14];
public:
	Court();
	void render(SDL_Renderer*);
}; 
