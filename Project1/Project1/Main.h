#pragma once

#include "iostream"
#include <stdio.h>
#include <time.h>

#include "SDL.h"
#include "SDL_image.h"

#define fieldsX 30
#define fieldsY 30

enum FieldState 
{
	Empty = 0,
	Bomb1 = 1,
	Bomb2 = 2,
	Bomb3 = 3,
	Bomb4 = 4,
	Bomb5 = 5,
	Bomb6 = 6,
	Bomb7 = 7,
	Bomb8 = 8,
	Bomb = 9,
	Flag = 10,
	Hidden = 11
};

typedef struct 
{
	int initialState;
	int state;
} Field;

void initGame(SDL_Renderer *renderer, SDL_Texture *image, Field fields[]);
void initFieldOffSet(SDL_Renderer *renderer, SDL_Texture *image, int x, int y);