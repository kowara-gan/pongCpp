#pragma once
#include "SDL.h"
#include <vector>
using namespace std;
class Alphanum
{
public:
	//SDL2で作る文字の配列を作る
	static vector<SDL_Rect> getAlphanum(int,int,int);
	//SDL2で作るった文字をレンダーに描画
	static void write(SDL_Renderer*, vector<SDL_Rect>);
	//SDL2で文字を作ってレンダーに描画
	static void writeAlphanum(SDL_Renderer*, int,int,int);
};