// ToDo
// implement bit turn off/on system somehow 
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

typedef uint8_t BYTE;
struct Types {
	int program_counter : 12;
};

#define PIXEL_SIZE 25
#define WINDOW_WIDTH 64 * PIXEL_SIZE
#define WINDOW_HEIGHT 32 * PIXEL_SIZE
BYTE pixels[WINDOW_HEIGHT*WINDOW_WIDTH*PIXEL_SIZE] = {0};

bool* byte_to_bits(BYTE byte) {
	static bool bits[8];
	for (int i = 0; i < 8; i++) {
		bits[i] = (byte&1);
		byte = byte>>1;
	}
	return bits;
}

void draw_pixel(int start_x, int start_y) {
	int index;
	for (int y = start_y; y < start_y+30; y++) {
		for (int x = start_x; x < start_x+30; x++) {
			index = x+y*WINDOW_WIDTH;
			*(pixels+index) = 255;
		}
	}
}

void draw_shape(BYTE* first_byte, int start_x, int start_y, SDL_Renderer *renderer, int num_of_bytes) {
	bool sprite[num_of_bytes];
	bool* cur_bits;

	for (int i = 0; i < num_of_bytes/8; i++) {
		cur_bits = byte_to_bits(*(first_byte+i));
		for (int j = 0; j < 8; j++) {
			sprite[i*8+j] = *(cur_bits+j);
		}
	}

	for (int i = 0; i < num_of_bytes; i++) {
		if (sprite[i]) {
			for (int y = start_y; y < start_y+PIXEL_SIZE; y++) {
				for (int x = start_x; x < start_x+PIXEL_SIZE; x++) {
					SDL_RenderDrawPoint(renderer, x, y);
					draw_pixel(x, y);
				}
			}
		}

		if ((i+1) % 8 == 0) {
			start_y += PIXEL_SIZE;
			start_x -= PIXEL_SIZE*7;
		}
		else {
			start_x += PIXEL_SIZE;
		}
	}
}

int main(int argc, char* args[]) {
	SDL_Event event;
	SDL_Window* window = SDL_CreateWindow("Title", 0, 20, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, NULL);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_Init(SDL_INIT_VIDEO);

	BYTE memory[2048] = {
		// Fonts
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
		// Instructions
		// Opcodes are 2 bytes, 2 hex digits = 1 byte
		// First byte for the instruction and the second one for the value (I think)
		0xA0, 0x0F, // Set index register to value F 
		// Is index register the same things as program counter???
		// No, I think it just points to a memory location??
	}; 

	struct Types current_counter = {program_counter:80};
	signed int index_register = 0;

	draw_pixel(100, 100);
	SDL_UpdateTexture(texture, NULL, pixels, WINDOW_WIDTH);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	int cur_instruction;
	int cur_value;

	while (1) {
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;

		cur_instruction = memory[current_counter.program_counter];
		cur_value = memory[current_counter.program_counter+1];
		// I don't remember why I did this so I'll just ignore it for now
		// cur_instruction = (cur_instruction>>4);

		switch (cur_instruction) {
			case 0xA0:
				index_register = cur_value;
				printf("Setting index register to: %d", memory[current_counter.program_counter+1]);
				printf("Index register value: %d\n", index_register);
				break;
			default:
				break;
		}
		if (current_counter.program_counter < 200) {
			current_counter.program_counter++;
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(texture);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}
