// ToDo
// pixel array is separate from memory
// draw function needs to be redone using pixels array
// should I make pixels and memory global?
//
// 8 bit pixels is best I can do
// PIXEL DRAWN LETS GOOO
//
#include <iostream>
#include <SDL.h>
using namespace std;

typedef uint8_t BYTE;
struct Types {
	int program_counter : 12;
};

const int PIXEL_SIZE = 25;
const int WINDOW_WIDTH = 64 * PIXEL_SIZE;
const int WINDOW_HEIGHT = 32 * PIXEL_SIZE;

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

void draw_pixel(int start_x, int start_y, BYTE* pixels) {
	int index;
	for (int y = start_y; y < start_y+30; y++) {
		for (int x = start_x; x < start_x+30; x++) {
			index = x+y*WINDOW_WIDTH;
			*(pixels+index) = 255;
		}
	}
}

int main(int argc, char* args[]) {
	SDL_Event event;
	SDL_Window* window = SDL_CreateWindow("Title", 0, 20, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, NULL);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_Init(SDL_INIT_VIDEO);

	Types current_counter = Types{program_counter:0};

	BYTE memory[2048] = {
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
	BYTE pixels[WINDOW_HEIGHT*WINDOW_WIDTH] = {0};

	draw_pixel(100, 100, pixels);
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
