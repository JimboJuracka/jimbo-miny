#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <unistd.h>

#include "game_logic.h"

// 9 x 9 - 10
// 16 x 16 - 40
// 16 x 30 - 99

// update_logic_result game_state = LOGIC_IDLE;
// bool time_is_running = false;
// uint32_t start_time;

bool init_logic(tileset* mine_field, int height, int width, int bombs){
    // allocation of memory
    mine_field->field = malloc(sizeof(tile*) * height);
    if(mine_field->field == NULL){
        return false;
    }
    for(int i = 0; i < height; i++){
        mine_field->field[i] = (tile*)malloc(sizeof(tile) * width);
        if(mine_field->field[i] == NULL){
            for(int k = i - 1; k >= 0; k--){
                free(mine_field->field[k]);
            }
            return false;
        }
    }
    mine_field->height = height;
    mine_field->width = width;
    mine_field->bombs = bombs;
    mine_field->bombs_remaining = bombs;
    mine_field->time = 0;
    mine_field->time_is_running = false;
    mine_field->state = LOGIC_START;

    hide_tiles(mine_field);
    return true;
};

void hide_tiles(tileset* mine_field){
    for(int i = 0; i < mine_field->height; i++){
        for(int j = 0; j < mine_field->width; j++){
            mine_field->field[i][j].mask = HIDDEN;
        }
    }
}

unsigned int generateComplexSeed() {
    unsigned int seed = time(NULL) ^ getpid() ^ (uintptr_t)&seed ^ SDL_GetTicks();
    return seed;
}

// this function places mines, set numbers around them, only exists outside of init_logic so player can reset game
void start_game(tileset* mine_field, int start_x, int start_y){
    srand(generateComplexSeed());

    for(int i = 0; i < mine_field->height; i++){
        for(int j = 0; j < mine_field->width; j++){
            mine_field->field[i][j].value = BLANK;
        }
    }

    // setting up mines on random locations
    int new_x;
    int new_y;
    int bombs_left_to_place = mine_field->bombs;
    if(mine_field->bombs >= mine_field->height * mine_field->width - 1){
        for(int i = 0; i < mine_field->height; i++){
            for(int j = 0; j < mine_field->width; j++){
                if(i != start_x && j != start_y){
                    mine_field->field[i][j].value = BOMB;
                }
            }
        }
    }else{
        while(bombs_left_to_place > 0){
            new_x = rand() % mine_field->height;
            new_y = rand() % mine_field->width;
            if(mine_field->field[new_x][new_y].value != BOMB && new_x != start_x && new_y != start_y){
                mine_field->field[new_x][new_y].value = BOMB;
                bombs_left_to_place--;
            }
        }
    }

    // setting up values around bombs
    int bombs_around;
    for(int i = 0; i < mine_field->height; i++){
        for(int j = 0; j < mine_field->width; j++){
            if(mine_field->field[i][j].value != BOMB){
                bombs_around = 0;
                for(int k = i - 1; k <= i + 1; k++){
                    for(int l = j - 1; l <= j + 1; l++){
                        if(k >= 0 && k < mine_field->height && l >= 0 && l < mine_field->width && mine_field->field[k][l].value == BOMB){
                            bombs_around++;
                        }
                    }
                }
                mine_field->field[i][j].value = bombs_around;
            }
        }
    }

    mine_field->state = LOGIC_IDLE;
}

void print_game(tileset* mine_field){
    for(int i = 0; i < mine_field->height; i++){
        for(int j = 0; j < mine_field->width; j++){
            if(mine_field->field[i][j].mask == HIDDEN){
                printf("\x1b[97m" "# " "\x1b[0m");
            }else if(mine_field->field[i][j].mask == FLAG){
                printf("\x1b[97m" "? " "\x1b[0m");
            }else{
                switch(mine_field->field[i][j].value){
                    case 0:
                        printf("\x1b[40m" ". " "\x1b[0m");
                        break;
                    case 1:
                        printf("\x1b[96m" "1 " "\x1b[0m");
                        break;
                    case 2:
                        printf("\x1b[32m" "2 " "\x1b[0m");
                        break;
                    case 3:
                        printf("\x1b[91m" "3 " "\x1b[0m");
                        break;
                    case 4:
                        printf("\x1b[93m" "4 " "\x1b[0m");
                        break;
                    case 5:
                        printf("\x1b[35m" "5 " "\x1b[0m");
                        break;
                    case 6:
                        printf("\x1b[36m" "6 " "\x1b[0m");
                        break;
                    case 7:
                        printf("\x1b[95m" "7 " "\x1b[0m");
                        break;
                    case 8:
                        printf("\x1b[90m" "8 " "\x1b[0m");
                        break;
                    case BOMB:
                        printf("\x1b[37m" "* " "\x1b[0m");
                        break;
                    default:
                }
            }
        }

        fprintf(stdout, "\t");

        for(int j = 0; j < mine_field->width; j++){
            if(mine_field->field[i][j].mask == CLEAR){
                printf("_ ");
            }else{
                printf("# ");
            }
        }

        fprintf(stdout, "\t");

        for(int j = 0; j < mine_field->width; j++){
            if(mine_field->field[i][j].value == BOMB){
                printf("+ ");
            }else{
                printf("%d ", (int)mine_field->field[i][j].value);
            }
        }

        fprintf(stdout, "\n");
    }
}

void reveal_blank_tiles(tileset* mine_field, int x, int y){
    mine_field->field[x][y].mask = CLEAR;
    if(mine_field->field[x][y].value == BLANK){
        for(int i = x - 1; i <= x + 1; i++){
            for(int j = y - 1; j <= y + 1; j++){
                if(i >= 0 && i < mine_field->height && j >= 0 && j < mine_field->width && mine_field->field[i][j].mask == HIDDEN && !(i == x && j == y)){
                    reveal_blank_tiles(mine_field, i, j);
                }
            }
        }
    }
}

void update_logic(tileset* mine_field, int x, int y, int key){
    // if out of bounds, do nothing
    if(x >= mine_field->height || x < 0 || y >= mine_field->width || y < 0){
        return;
    }

    // if game was already lost or won, do nothing
    if(mine_field->state == LOGIC_LOSS){
        return;
    }else if(mine_field->state == LOGIC_WIN){
        return;
    }

    // start the timer if it wasnt already running
    if(!mine_field->time_is_running){
        mine_field->time_is_running = true;
        mine_field->start_time = SDL_GetTicks();
    }

    // if flag is placed/displaced
    if(key == RIGHT_KEY){
        if(mine_field->field[x][y].mask == HIDDEN){
            mine_field->field[x][y].mask = FLAG;
            mine_field->bombs_remaining--;
        }else if(mine_field->field[x][y].mask == FLAG){
            mine_field->field[x][y].mask = HIDDEN;
            mine_field->bombs_remaining++;
        }
        return;
    }

    // if this is the first tile to be cleared, generate bombs and start the game
    if(mine_field->state == LOGIC_START){
        start_game(mine_field, x, y);
    }

    // if the clicked tile is already clear, try to clear all eight tiles around it
    if(mine_field->field[x][y].mask == CLEAR){
        // first we need to count all flags around the tile
        int count_flags = 0;
        for(int i = x - 1; i <= x+1; i++){
            for(int j = y - 1; j <= y+1; j++){
                if(i >= 0 && i < mine_field->height && j >= 0 && j < mine_field->width && mine_field->field[i][j].mask == FLAG){
                    count_flags++;
                }
            }
        }
        // if the number of counted flags is equal to the value of tile, we can clear all tiles around
        if(count_flags == mine_field->field[x][y].value){
            for(int i = x - 1; i <= x+1; i++){
                for(int j = y - 1; j <= y+1; j++){
                    if(i >= 0 && i < mine_field->height && j >= 0 && j < mine_field->width && mine_field->field[i][j].mask != FLAG){
                        mine_field->field[i][j].mask = CLEAR;
                        // if any of the cleared tiles has a bomb, mark it
                        if(mine_field->field[i][j].value == BOMB){
                            mine_field->field[i][j].value = TRIGGERED_BOMB;
                            mine_field->state = LOGIC_LOSS;
                            mine_field->time_is_running = false;
                        }
                        // if any of the cleared tiles is blank, reveal other blank tiles
                        if(mine_field->field[i][j].value == BLANK){
                            reveal_blank_tiles(mine_field, i, j);
                        }
                    }
                }
            }

        }
        // if any bombs were triggered this way, return
        if(mine_field->state == LOGIC_LOSS){
            return;
        }
    }
    
    // if the clicked on tile was hidden and has bomb, the game is over
    if(mine_field->field[x][y].value == BOMB){
        mine_field->field[x][y].mask = CLEAR;

        // uncover the rest of the bombs
        for(int i = 0; i < mine_field->height; i++){
            for(int j = 0; j < mine_field->width; j++){
                if(mine_field->field[i][j].value == BOMB){
                    mine_field->field[i][j].mask = CLEAR;
                }
            }
        }
        mine_field->field[x][y].value = TRIGGERED_BOMB;
        mine_field->time_is_running = false;
        mine_field->state = LOGIC_LOSS;
        return;
    }
    
    //otherwise, reveal all the blank tiles, if any
    reveal_blank_tiles(mine_field, x, y);

    // if there are still any hidden tiles without bomb, the game continues
    for(int i = 0; i < mine_field->height; i++){
        for(int j = 0; j < mine_field->width; j++){
            if((mine_field->field[i][j].mask == HIDDEN || mine_field->field[i][j].mask == FLAG) && mine_field->field[i][j].value != BOMB){
                mine_field->state = LOGIC_IDLE;
                return;
            }
        }
    }

    // otherwise, the player won, and we can mark all the bombs
    for(int i = 0; i < mine_field->height; i++){
        for(int j = 0; j < mine_field->width; j++){
            if(mine_field->field[i][j].mask == HIDDEN && mine_field->field[i][j].value == BOMB){
                mine_field->field[i][j].mask = FLAG;
            }
        }
    }
    mine_field->bombs_remaining = 0;
    mine_field->time_is_running = false;
    mine_field->state = LOGIC_WIN;
    return;
}

void destroy_logic(tileset* mine_field){
    if(mine_field != NULL){
        for(int i = 0; i < mine_field->height; i++){
            free(mine_field->field[i]);
        }
        free(mine_field->field);
    }
}