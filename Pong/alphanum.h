#pragma once
#include "SDL.h"
#include <vector>
using namespace std;
class Alphanum
{
public:
	//SDL2�ō�镶���̔z������
	static vector<SDL_Rect> getAlphanum(int,int,int);
	//SDL2�ō����������������_�[�ɕ`��
	static void write(SDL_Renderer*, vector<SDL_Rect>);
	//SDL2�ŕ���������ă����_�[�ɕ`��
	static void writeAlphanum(SDL_Renderer*, int,int,int);
};