// DONE
// 8 bit pixels is best I can do
//
#include <iostream>
#include <SDL.h>
using namespace std;

typedef uint8_t BYTE;

int main(int argc, char* args[]) {
	const int WINDOW_WIDTH = 600;
	const int WINDOW_HEIGHT = 600;
	SDL_Event event;
	SDL_Window* window = SDL_CreateWindow("Title", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, NULL);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_Init(SDL_INIT_VIDEO);

	BYTE pixels[WINDOW_HEIGHT*WINDOW_WIDTH*1] = {0};

	int start_x = 100;
	int start_y = 100;
	int index;

	for (int y = start_y; y < start_y+30; y++) {
		for (int x = start_x; x < start_x+30; x++) {
			index = (x+y*WINDOW_WIDTH)*1;
			pixels[index] = 255;
			//pixels[index+1] = 255;
			//pixels[index+2] = 255;
		}
	}

	SDL_UpdateTexture(texture, NULL, pixels, WINDOW_WIDTH*1);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	while (1) {
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(texture);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}
