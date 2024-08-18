#ifndef GAME_GRAPHICS
#define GAME_GRAPHICS

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "game_logic.h"

#define TILE_SIZE 16*2
#define BORDER_SIZE 12*2
#define HEADER_HEIGHT 30*2
#define BOARDER_MIDDLE 15*2
#define FACE_SIZE 26*2
#define DIGIT_HEIGHT 25*2
#define DIGIT_WIDTH 13*2

extern bool game_quit;

bool init_graphics(tileset* logic);

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);

SDL_Texture* numbertoTexture(int number);

bool process_input(tileset* logic, int* row, int* col, int* key);

void update_graphics(tileset* logic);

void render_graphics(void);

void destroy_graphics(void);

#endif