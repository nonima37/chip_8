// need to figure out how to render by sendind a pixel array like i did in golang
// so that i can simulate the behaviour of bit flipping to change colors
// like it was in chip 8
#include <iostream>
#include <SDL.h>
using namespace std;

typedef unsigned char BYTE;
struct Types {
	int program_counter : 12;
};

bool* byte_to_bits(BYTE byte) {
	static bool bits[8];
	for (int i = 0; i < 8; i++) {
		bits[i] = (byte&1);
		byte = byte>>1;
	}
	return bits;
}

void draw_number(BYTE* first_byte, int start_x, int start_y, int size, SDL_Renderer *renderer) {
	bool sprite[40];
	bool* cur_bits;

	for (int i = 0; i < 5; i++) {
		cur_bits = byte_to_bits(*(first_byte+i));
		for (int j = 0; j < 8; j++) {
			sprite[i*8+j] = *(cur_bits+j);
		}
	}

	for (int i = 0; i < 40; i++) {
		if (sprite[i]) {
			for (int y = start_y; y < start_y+size; y++) {
				for (int x = start_x; x < start_x+size; x++) {
					SDL_RenderDrawPoint(renderer, x, y);
				}
			}
		}

		if ((i+1) % 8 == 0) {
			start_y += size;
			start_x -= size*7;
		}
		else {
			start_x += size;
		}
	}
}

int main(int argc, char* args[]) {
	const int WINDOW_WIDTH = 600;
	const int WINDOW_HEIGHT = 600;
	SDL_Event event;
	SDL_Window* window = SDL_CreateWindow("Title", 100, 100, WINDOW_HEIGHT, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

	unsigned char pixels[WINDOW_HEIGHT*WINDOW_WIDTH*3];

	int start_x = 100;
	int start_y = 100;
	int index;
	for (int y = start_y; y < 300; y++) {
		for (int x = start_x; x < 300; x++) {
			index = (x+y*WINDOW_WIDTH)*3;
			pixels[index] = 255;
			pixels[index+1] = 255;
			pixels[index+2] = 255;
		}
	}

	SDL_UpdateTexture(texture, NULL, pixels, WINDOW_WIDTH*3);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	BYTE memory[4096] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, 
		0x20, 0x60, 0x20, 0x20, 0x70,
		0xF0, 0x10, 0xF0, 0x80, 0xF0, 
		0xF0, 0x10, 0xF0, 0x10, 0xF0, 
		0x90, 0x90, 0xF0, 0x10, 0x10,
		0xF0, 0x80, 0xF0, 0x10, 0xF0,
		0xF0, 0x80, 0xF0, 0x90, 0xF0,
		0xF0, 0x10, 0x20, 0x40, 0x40,
		0xF0, 0x90, 0xF0, 0x90, 0xF0,
		0xF0, 0x90, 0xF0, 0x10, 0xF0,
		0xF0, 0x90, 0xF0, 0x90, 0x90,
		0xE0, 0x90, 0xE0, 0x90, 0xE0,
		0xF0, 0x80, 0x80, 0x80, 0xF0,
		0xE0, 0x90, 0x90, 0x90, 0xE0,
		0xF0, 0x80, 0xF0, 0x80, 0xF0,
		0xF0, 0x80, 0xF0, 0x80, 0x80,
	}; 

	Types current_counter = Types{program_counter:0};

	while (1) {
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	system("pause");
	return EXIT_SUCCESS;
}
