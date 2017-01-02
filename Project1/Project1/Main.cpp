#include "Main.h"

using namespace std;

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	cout << "testele mele !";

	SDL_Window *window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, fieldsX * 30, fieldsY * 30, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture *emptyImage = IMG_LoadTexture(renderer, "../images/empty.bmp");

	initGame(renderer, emptyImage);

	displayFields(renderer);

	SDL_Event event;
	bool running = true;

	while (running == true)
	{
		if (SDL_PollEvent(&event))
		{

		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}

void initGame(SDL_Renderer *renderer, SDL_Texture *image)
{
	SDL_RenderClear(renderer);

	for (int i = 0; i < fieldsX; i++)
	{
		for (int j = 0; j < fieldsY; j++)
		{
			fields[i + j*fieldsX].initialState = Empty;
			fields[i + j*fieldsX].state = Hidden;

			initFieldOffSet(renderer, image, i, j);
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

		if (fields[x + y * fieldsX].hasBomb == true)
		{
			i--;
		} 
		else 
		{
			if (x != 0 || y != 0)
			{
				fields[x + y * fieldsX].hasBomb = true;

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
			fields[i + j * fieldsX].bombNumber++;
		}
	}
}

void initFieldOffSet(SDL_Renderer *renderer, SDL_Texture *image, int x, int y)
{
	SDL_Rect destinationRect;

	destinationRect.x = x*fieldsX;
	destinationRect.y = y*fieldsX;
	destinationRect.w = fieldsX;
	destinationRect.h = fieldsX;

	SDL_RenderCopy(renderer, image, NULL, &destinationRect);
}

int getAverage(int x, int y)
{
	int offset = rand() % (y - x);
	return x + offset;
}

void displayFields(SDL_Renderer *renderer)
{
	SDL_RenderClear(renderer);

	for (int i = 0; i < fieldsX; i++)
	{
		for (int j = 0; j < fieldsY; j++)
		{
			Field field = fields[i + j * fieldsY];

			SDL_Texture *image = getImageFromField(renderer, field);
			
			if (image != NULL)
			{
				initFieldOffSet(renderer, image, i, j);
			}
		}
	}
	
	SDL_RenderPresent(renderer);
}

SDL_Texture* getImageFromField(SDL_Renderer *renderer, Field field)
{
	SDL_Texture *image = NULL;

	if (field.hasBomb == true)
	{
		image = IMG_LoadTexture(renderer, "../images/bomb.bmp");
	}
	else if (field.initialState == Empty)
	{
		image = IMG_LoadTexture(renderer, "../images/empty.bmp");

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
	
	return image;
}