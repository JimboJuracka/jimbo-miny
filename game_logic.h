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

// structure contains all the information about the current state of the game
typedef struct {
    // static values:
    int height;     // number of tiles in each column
    int width;      // number of tiles in each row
    int bombs;      //number of bombs in the game
    
    //dynamic values:
    int bombs_remaining;    // number of bombs left to mark (can go below 0)
    int time;               // time in seconds
    uint32_t start_time;    // time when the first tile was cleared (in miliseconds)
    bool time_is_running;   // true if the countdown is running, otherwise false
    game_state state;       // state the game is currently in

    tile** field;           // pointer to all the tiles
} tileset;

bool init_logic(tileset* mine_field, int height, int width, int n_mines);

void hide_tiles(tileset* mine_field);

unsigned int generateComplexSeed();

void start_game(tileset* mine_field, int start_x, int start_y);

void update_logic(tileset* mine_field, int x, int y, int key);

void print_game(tileset* mine_field);

void destroy_logic(tileset* mine_field);

#endif