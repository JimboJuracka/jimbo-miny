#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <getopt.h>

#include "game_logic.h"
#include "game_graphics.h"


tileset logic;
bool game_quit = false;
int x, y, key;
int x_size, y_size, bombs;
const char help_message[] = 
"Select a difficulty:\n"
"-b BEGINNER - size 9x9   bombs: 10\n"
"-m MEDIUM   - size 16x16 bombs: 40\n"
"-e EXPERT   - size 16x30 bombs: 99\n"
"-c CUSTOM <height> <width> <number of bombs> (WARNING: Easily breakable)\n\n"
"You can always exit the game by closing the window, or pressing ESC key.\n"
;

int main(int argc, char* argv[]){
    char c;
    if(argc == 1){
        fprintf(stdout, help_message);
        return 0;
    }
    while((c = getopt(argc, argv, "bmec")) != -1){
        switch(c){
            case 'b':
                x_size = 9; y_size = 9; bombs = 10;
                break;

            case 'm':
                x_size = 16; y_size = 16; bombs = 40;
                break;

            case 'e':
                x_size = 16; y_size = 30; bombs = 99;
                break;

            case 'c':
                if(argc - optind < 3){
                    fprintf(stdout, "Too few arguments\n");
                    fprintf(stdout, help_message);
                    return 1;
                }
                x_size = atoi(argv[optind]);
                y_size = atoi(argv[optind + 1]);
                bombs = atoi(argv[optind + 2]);
                if(x_size <= 0 || y_size <= 0 || bombs <= 0){
                    fprintf(stdout, "Invalid argument\n");
                    fprintf(stdout, help_message);
                    return 1;
                }
                break;
            default:
                fprintf(stdout, help_message);
                return 0;
                break;
        }
    }

    init_logic(&logic, x_size, y_size, bombs);
    game_quit = init_graphics(&logic);
    if(!game_quit){
        return 1;
    }

    while(game_quit){
        if(process_input(&logic, &x, &y, &key)){
            update_logic(&logic, x, y, key);
            update_graphics(&logic);
            x = -1;
            y = -1;
        }
    }
    
    destroy_graphics();
    destroy_logic(&logic);
    
	return 0;
}
