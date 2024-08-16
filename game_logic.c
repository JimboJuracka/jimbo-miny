#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#include "game_logic.h"

// 9 x 9 - 10
// 16 x 16 - 40
// 16 x 30 - 99

update_logic_result game_state = LOGIC_IDLE;
bool time_is_running = false;
uint32_t start_time;

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
    setup_game(mine_field);

    return true;
};

// this function places mines, set numbers around them, only exists outside of init_logic so player can reset game
void setup_game(tileset* mine_field){
    mine_field->bombs_remaining = mine_field->bombs;
    time_is_running = false;
    mine_field->time = 0;

    for(int i = 0; i < mine_field->height; i++){
        for(int j = 0; j < mine_field->width; j++){
            mine_field->field[i][j].mask = HIDDEN;
            mine_field->field[i][j].value = BLANK;
        }
    }

    // setting up mines on random locations
    int new_x;
    int new_y;
    int bombs_left_to_place = mine_field->bombs;
    if(mine_field->bombs >= mine_field->width * mine_field->height){
        for(int i = 0; i < mine_field->height; i++){
            for(int j = 0; j < mine_field->width; j++){
                mine_field->field[i][j].value = BOMB;
            }
        }
    }else{
        while(bombs_left_to_place > 0){
            new_x = rand() % mine_field->height;
            new_y = rand() % mine_field->width;
            if(mine_field->field[new_x][new_y].value != BOMB){
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

update_logic_result update_logic(tileset* mine_field, int x, int y, int key){
    // if out of bounds
    if(x >= mine_field->height || x < 0 || y >= mine_field->width || y < 0){
        return LOGIC_IDLE;
    }

    // if game was already lost or won
    if(game_state == LOGIC_LOSS){
        return LOGIC_LOSS;
    }else if(game_state == LOGIC_WIN){
        return LOGIC_WIN;
    }

    // start timer if not already running
    if(!time_is_running){
        start_time = SDL_GetTicks();
    }
    time_is_running = true;

    // if flag is placed/displaced
    if(key == RIGHT_KEY){
        if(mine_field->field[x][y].mask == HIDDEN){
            mine_field->field[x][y].mask = FLAG;
            mine_field->bombs_remaining--;
        }else if(mine_field->field[x][y].mask == FLAG){
            mine_field->field[x][y].mask = HIDDEN;
            mine_field->bombs_remaining++;
        }
        return LOGIC_IDLE;
    }

    // if the clicked tile is already clear
    if(mine_field->field[x][y].mask == CLEAR){
        // first we need to count all flags around the tile
        int count_flags = 0;
        int game_lost = 0;
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
                            game_lost = 1;
                        }
                        // if any of the cleared tiles is blank, reveal other blank tiles
                        if(mine_field->field[i][j].value == BLANK){
                            reveal_blank_tiles(mine_field, i, j);
                        }
                    }
                }
            }

        }
        // if any bomb were triggered this way, return loss
        if(game_lost == 1){
            time_is_running = false;
            return LOGIC_LOSS;
        }
    }
    

    // if the clicked on tile has bomb, the game is over
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
        time_is_running = false;
        return LOGIC_LOSS;
    }
    
    //otherwise reveal the blank tiles, if any
    reveal_blank_tiles(mine_field, x, y);

    // if there are any hidden tiles without bomb, the game continues
    for(int i = 0; i < mine_field->height; i++){
        for(int j = 0; j < mine_field->width; j++){
            if((mine_field->field[i][j].mask == HIDDEN || mine_field->field[i][j].mask == FLAG) && mine_field->field[i][j].value != BOMB){
                return LOGIC_IDLE;
            }
        }
    }

    // otherwise the player won, we can mark all the bombs
    for(int i = 0; i < mine_field->height; i++){
        for(int j = 0; j < mine_field->width; j++){
            if(mine_field->field[i][j].mask == HIDDEN && mine_field->field[i][j].value == BOMB){
                mine_field->field[i][j].mask = FLAG;
            }
        }
    }
    time_is_running = false;
    return LOGIC_WIN;
}

void destroy_logic(tileset* mine_field){
    if(mine_field != NULL){
        for(int i = 0; i < mine_field->height; i++){
            free(mine_field->field[i]);
        }
        free(mine_field->field);
    }
}