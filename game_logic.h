#ifndef GAME_LOGIC
#define GAME_LOGIC

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    LOGIC_IDLE,
    LOGIC_LOSS,
    LOGIC_WIN,
    LOGIC_ERR
} update_logic_result;

typedef struct {
    char mask;
    char value;
} tile;

typedef struct {
    int height;
    int width;
    int bombs;    //number of bombs
    int bombs_remaining;
    tile** field;
} tileset;

// 0-------Y
// | 0 0 0
// | 0 0 0 
// | 0 0 0 
// X

extern update_logic_result game_state;

bool init_logic(tileset* mine_field, int height, int width, int n_mines);

update_logic_result update_logic(tileset* mine_field, int x, int y, int key);

void setup_game(tileset* mine_field);

void print_game(tileset* mine_field);

void destroy_logic(tileset* mine_field);

#endif