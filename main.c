#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>

#include "game_logic.h"
#include "game_graphics.h"


tileset logic;
bool game_quit = false;
int x;
int y;
int key;

int main(void){
    srand(time(NULL));
    init_logic(&logic, 16, 9, 10);
    game_quit = init_graphics(&logic);
    if(!game_quit){
        return 1;
    }

    while(game_quit){
        if(process_input(&logic, &x, &y, &key)){
            game_state = update_logic(&logic, x, y, key);
            //print_game(&logic);
        }
        update_graphics(&logic);
    }
    
    destroy_graphics();
    destroy_logic(&logic);
    
	return 0;
}
