#include "Main.h"

using namespace std;

int main(int argc, char** argv)
{
	int difficulty = 1;

	if (difficulty == 1)
	{
		fieldsX = 8;
		fieldsY = 8;

		maxBombs = 10;
	}
	else if (difficulty == 2)
	{
		fieldsX = 16;
		fieldsY = 16;

		maxBombs = 40;
	}
	else if (difficulty == 3)
	{
		fieldsX = 31;
		fieldsY = 16;

		maxBombs = 99;
	}

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, fieldsX * fieldWidth, fieldsY * fieldWidth, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	return startGame(window, renderer);
}

// MARK: - Init Functions

bool startGame(SDL_Window *window, SDL_Renderer *renderer)
{
	initGame(renderer);

	displayFields(renderer);

	SDL_Event event;
	int running = 1;

	// if running 1 - nothing happens
	// 2 - user lost game, game waits a click to begin

	while (running > 0)
	{
		if (SDL_PollEvent(&event))
		{
			running = solveEvent(event, renderer, running);			
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}

void initGame(SDL_Renderer *renderer)
{
	SDL_RenderClear(renderer);

	fieldsRevealed = 0;
	numberOfEmptyFields = fieldsX * fieldsY;

	SDL_Texture *emptyImage = IMG_LoadTexture(renderer, "../images/empty.bmp");

	for (int i = 0; i < fieldsX; i++)
	{
		for (int j = 0; j < fieldsY; j++)
		{
			fields[i][j].hasBomb = false;
			fields[i][j].bombNumber = 0;
			fields[i][j].initialState = Empty;
			fields[i][j].state = Hidden;

			initFieldOffSet(renderer, emptyImage, i, j);
		}
	}

	initBombs();

	SDL_RenderPresent(renderer);
}

void initBombs()
{
	for (int i=0; i<maxBombs; i++) 
	{
		int x = getAverage(0, fieldsX);
		int y = getAverage(0, fieldsY);

		if (fields[x][y].hasBomb == true)
		{
			i--;
		} 
		else 
		{
			if (x != 0 || y != 0)
			{
				fields[x][y].hasBomb = true;
				fields[x][y].state = Hidden;
				fields[x][y].initialState = Bomb;

				numberOfEmptyFields--;

				initAdjacents(x, y);
			}
		}
	}
}

void initAdjacents(int x, int y)
{
	int xDown = x + 1;
	int xUp = x - 1;

	int yDown = y + 1;
	int yUp = y - 1;

	if (xUp < 0)
	{
		xUp = 0;
	}

	if (xDown >= fieldsX)
	{
		xDown = fieldsX - 1;
	}

	if (yUp < 0)
	{
		yUp = 0;
	}

	if (yDown >= fieldsY - 1)
	{
		yDown = fieldsY - 1;
	}

	for (int i = xUp; i <= xDown; i++)
	{
		for (int j = yUp; j <= yDown; j++)
		{
			fields[i][j].bombNumber++;
			fields[i][j].initialState = Number;
		}
	}
}

void initFieldOffSet(SDL_Renderer *renderer, SDL_Texture *image, int x, int y)
{
	SDL_Rect destinationRect;

	destinationRect.x = x*fieldWidth;
	destinationRect.y = y*fieldWidth;
	destinationRect.w = fieldWidth;
	destinationRect.h = fieldWidth;

	SDL_RenderCopy(renderer, image, NULL, &destinationRect);
}

void displayFields(SDL_Renderer *renderer)
{
	SDL_RenderClear(renderer);

	for (int i = 0; i < fieldsX; i++)
	{
		for (int j = 0; j < fieldsY; j++)
		{
			Field field = fields[i][j];

			SDL_Texture *image = getImageFromField(renderer, field);
			
			if (image != NULL)
			{
				initFieldOffSet(renderer, image, i, j);
			}
		}
	}
	
	SDL_RenderPresent(renderer);
}

// MARK: - Event Checkers

int solveEvent(SDL_Event event, SDL_Renderer *renderer, int running)
{
	switch (event.type)
	{
		case SDL_QUIT:
		{
			return 0;
			break;
		}

		case SDL_KEYDOWN: 
		{
			SDL_Keysym pressedKey = event.key.keysym;

			if (pressedKey.sym == SDLK_n)
			{
				initGame(renderer);

				return 1;

				break;
			}
		}

		case SDL_MOUSEBUTTONDOWN:
		{
			SDL_MouseButtonEvent pressedButton = event.button;

			int x = pressedButton.x / fieldWidth;
			int y = pressedButton.y / fieldWidth;

			if (fields[x][y].state != Hidden && fields[x][y].state != Flag)
			{
				return 1;

				break;
			}

			if (pressedButton.button == SDL_BUTTON_LEFT)
			{
				if (fields[x][y].hasBomb == true)
				{
					gameLost(renderer);

					MessageBox(NULL, "You have lost", NULL, NULL);

					initGame(renderer);

					return 1;

					break;
				}
				else
				{
					checkAdjacents(x, y, 0, renderer);
				
					displayFields(renderer);

					return 1;

					break;
				}
			}
			else
			{
				if (fields[x][y].state == Flag)
				{
					fields[x][y].state = Hidden;
				}
				else 
				{
					fields[x][y].state = Flag;
				}

				displayFields(renderer);

				return 1;

				break;
			}
		}
	}

	return 1;
}

void checkAdjacents(int x, int y, int count, SDL_Renderer *renderer)
{
	if (fields[x][y].state == fields[x][y].initialState)
	{
		return;
	}

	fields[x][y].state = fields[x][y].initialState;

	if (fields[x][y].state != Bomb)
	{
		fieldsRevealed++;
	}

	if (fieldsRevealed == numberOfEmptyFields)
	{
		gameLost(renderer);

		MessageBox(NULL, "You have won", NULL, NULL);

		initGame(renderer);

		return;
	}

	if (fields[x][y].state != Empty)
	{
		return;
	}

	// left-up corner

	if (x - 1 > 0 && y - 1 > 0)
	{
		checkAdjacents(x - 1, y - 1, count + 1, renderer);
	}

	// up corner

	if (x - 1 > 0)
	{
		checkAdjacents(x - 1, y, count + 1, renderer);
	}

	// right-up corner

	if (x - 1 > 0 && y + 1 < fieldsY)
	{
		checkAdjacents(x - 1, y + 1, count + 1, renderer);
	}

	// right

	if (y + 1 < fieldsY)
	{
		checkAdjacents(x, y + 1, count + 1, renderer);
	}

	// right-down corner

	if (x + 1 < fieldsX && y + 1 < fieldsY)
	{
		checkAdjacents(x + 1, y + 1, count + 1, renderer);
	}

	// down

	if (x + 1 < fieldsX)
	{
		checkAdjacents(x + 1, y, count + 1, renderer);
	}

	// down-left

	if (x + 1 < fieldsX && y - 1 > 0)
	{
		checkAdjacents(x + 1, y - 1, count + 1, renderer);
	}

	// left

	if (y - 1 > 0)
	{
		checkAdjacents(x, y - 1, count + 1, renderer);
	}
}

void gameLost(SDL_Renderer *renderer)
{
	SDL_RenderClear(renderer);

	for (int i = 0; i < fieldsX; i++)
	{
		for (int j = 0; j < fieldsY; j++)
		{
			Field field = fields[i][j];

			if (field.hasBomb == true)
			{
				field.state = Bomb;
			}
			else if (field.initialState == Empty)
			{
				field.state = Empty;

				fieldsRevealed--;
			}
			else
			{
				field.state = field.initialState;

				fieldsRevealed--;
			}

			SDL_Texture *image = getImageFromField(renderer, field);

			if (image != NULL)
			{
				initFieldOffSet(renderer, image, i, j);
			}
		}
	}

	SDL_RenderPresent(renderer);
}

// MARK: - Aditional Functions

SDL_Texture* getImageFromField(SDL_Renderer *renderer, Field field)
{
	SDL_Texture *image = NULL;

	if (field.state == Hidden)
	{
		image = IMG_LoadTexture(renderer, "../images/empty.bmp");
	}
	else if (field.state == Bomb)
	{
		image = IMG_LoadTexture(renderer, "../images/bomb.bmp");
	}
	else if (field.state == Number)
	{
		if (field.bombNumber == 1)
		{
			image = IMG_LoadTexture(renderer, "../images/1.bmp");
		}
		else if (field.bombNumber == 2)
		{
			image = IMG_LoadTexture(renderer, "../images/2.bmp");
		}
		else if (field.bombNumber == 3)
		{
			image = IMG_LoadTexture(renderer, "../images/3.bmp");
		}
		else if (field.bombNumber == 4)
		{
			image = IMG_LoadTexture(renderer, "../images/4.bmp");
		}
		else if (field.bombNumber == 5)
		{
			image = IMG_LoadTexture(renderer, "../images/5.bmp");
		}
		else if (field.bombNumber == 6)
		{
			image = IMG_LoadTexture(renderer, "../images/6.bmp");
		}
		else if (field.bombNumber == 7)
		{
			image = IMG_LoadTexture(renderer, "../images/7.bmp");
		}
		else if (field.bombNumber == 8)
		{
			image = IMG_LoadTexture(renderer, "../images/8.bmp");
		}
	}
	else if (field.state == Flag)
	{
		image = IMG_LoadTexture(renderer, "../images/flag.bmp");
	}
	else if (field.state == Empty)
	{
		image = IMG_LoadTexture(renderer, "../images/revealed.bmp");
	}
	
	return image;
}

int getAverage(int x, int y)
{
	int offset = rand() % (y - x);
	return x + offset;
}