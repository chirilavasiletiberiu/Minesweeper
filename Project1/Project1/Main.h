#pragma once

#include "iostream"
#include <stdio.h>
#include <time.h>
#include <windows.h>

#include "SDL.h"
#include "SDL_image.h"

int fieldsX;
int fieldsY;

#define fieldWidth 20

int cellsNumber;
int maxBombs;
 
enum FieldState 
{
	Empty = 0,
	Flag = 1,
	Number = 2,
	Hidden = 3,
	Bomb = 4
};

typedef struct 
{
	int initialState;
	int state;
	int bombNumber;
	bool hasBomb;

} Field;

bool startGame(SDL_Window *window, SDL_Renderer *renderer);

void initGame(SDL_Renderer *renderer);
void initBombs();
void initAdjacents(int x, int y);
int getAverage(int x, int y);
void initFieldOffSet(SDL_Renderer *renderer, SDL_Texture *image, int x, int y);
void displayFields(SDL_Renderer *renderer);

void checkAdjacents(int x, int y, int count, SDL_Renderer *renderer);

SDL_Texture* getImageFromField(SDL_Renderer *renderer, Field field);

Field fields[31][31];

int fieldsRevealed;
int numberOfEmptyFields;

int solveEvent(SDL_Event event, SDL_Renderer *renderer, int running);

void gameLost(SDL_Renderer *renderer);