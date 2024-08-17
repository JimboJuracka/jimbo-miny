#ifndef GAME_LOGIC
#define GAME_LOGIC

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// tile.mask
#define HIDDEN 1
#define CLEAR 0
#define FLAG 2
// tile.value 
#define BLANK 0
#define BOMB -1
#define TRIGGERED_BOMB -2

#define LEFT_KEY 0
#define RIGHT_KEY 1

// return values for update function
typedef enum update_logic_result{
    LOGIC_IDLE,     // state in whitch game is when first tile was already cleared, but the game isn't over
    LOGIC_LOSS,     // lost state
    LOGIC_WIN,      // win state
    LOGIC_START     // state in whitch game is, if no tile has been cleared yet
} game_state;

typedef struct {
    char mask;
    char value;
} tile;

typedef struct {
    int height;
    int width;
    int bombs;    //number of bombs
    
    int bombs_remaining;
    int time;
    uint32_t start_time;
    bool time_is_running;
    game_state state;

    tile** field;
} tileset;

// 0-------Y
// | 0 0 0
// | 0 0 0 
// | 0 0 0 
// X

//extern update_logic_result game_state;
//extern bool time_is_running;
//extern uint32_t start_time;

bool init_logic(tileset* mine_field, int height, int width, int n_mines);

void hide_tiles(tileset* mine_field);

unsigned int generateComplexSeed();

void start_game(tileset* mine_field, int start_x, int start_y);

void update_logic(tileset* mine_field, int x, int y, int key);

void print_game(tileset* mine_field);

void destroy_logic(tileset* mine_field);

#endif