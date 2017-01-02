#pragma once

#include "iostream"
#include <stdio.h>
#include <time.h>

#include "SDL.h"
#include "SDL_image.h"

#define fieldsX 30
#define fieldsY 30

#define cellsNumber fieldsX * fieldsY
#define maxBombs cellsNumber/10

enum FieldState 
{
	Empty = 0,
	Flag = 1,
	Number = 2,
	Hidden = 3
};

typedef struct 
{
	int initialState;
	int state;
	int bombNumber;
	bool hasBomb;

} Field;

void initGame(SDL_Renderer *renderer, SDL_Texture *image);
void initBombs();
void initAdjacents(int x, int y);
int getAverage(int x, int y);
void initFieldOffSet(SDL_Renderer *renderer, SDL_Texture *image, int x, int y);
void displayFields(SDL_Renderer *renderer);

SDL_Texture* getImageFromField(SDL_Renderer *renderer, Field field);

Field fields[cellsNumber];