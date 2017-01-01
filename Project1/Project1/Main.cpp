#include "Main.h"

using namespace std;

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, fieldsX * 30, fieldsY * 30, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Field fields[fieldsX * fieldsY];

	SDL_Texture *emptyImage = IMG_LoadTexture(renderer, "../images/empty.bmp");

	initGame(renderer, emptyImage, fields);

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

void initGame(SDL_Renderer *renderer, SDL_Texture *image, Field fields[])
{
	SDL_RenderClear(renderer);

	for (int i = 0; i < fieldsX; i++)
	{
		for (int j = 0; j < fieldsY; j++)
		{
			fields[i + j*fieldsX].state = Hidden;
			initFieldOffSet(renderer, image, i, j);
		}
	}

	SDL_RenderPresent(renderer);
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