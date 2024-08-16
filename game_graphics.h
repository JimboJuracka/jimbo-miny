#ifndef GAME_GRAPHICS
#define GAME_GRAPHICS

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "game_logic.h"

#define TILE_SIZE 30
#define BORDER_AROUND_BOARD 20
#define BOARD_HEADER_SIZE 90
#define BOARDER_MIDDLE_SIZE 20
#define FACE_SIZE 50

extern bool game_quit;

bool init_graphics(tileset* logic);

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);

bool process_input(tileset* logic, int* row, int* col, int* key);

void update_graphics(tileset* logic);

void render_graphics(void);

void destroy_graphics(void);

#endif