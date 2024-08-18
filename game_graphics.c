#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "game_graphics.h"
#include "game_logic.h"

int window_height;
int window_width;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

//rectangles
SDL_Rect** rect_tiles;
SDL_Rect rect_face;
SDL_Rect rect_header;

SDL_Rect rect_top_left;
SDL_Rect rect_top_right;
SDL_Rect rect_middle_left;
SDL_Rect rect_middle_right;
SDL_Rect rect_bottom_left;
SDL_Rect rect_bottom_right;

SDL_Rect rect_top;
SDL_Rect rect_middle;
SDL_Rect rect_bottom;
SDL_Rect rect_left;
SDL_Rect rect_right;

SDL_Rect rect_bombs_1;
SDL_Rect rect_bombs_10;
SDL_Rect rect_bombs_100;

SDL_Rect rect_time_1;
SDL_Rect rect_time_10;
SDL_Rect rect_time_100;

// textures
SDL_Texture* tex_bomb;
SDL_Texture* tex_tile0;
SDL_Texture* tex_tile1;
SDL_Texture* tex_tile2;
SDL_Texture* tex_tile3;
SDL_Texture* tex_tile4;
SDL_Texture* tex_tile5;
SDL_Texture* tex_tile6;
SDL_Texture* tex_tile7;
SDL_Texture* tex_tile8;
SDL_Texture* tex_tilefull;
SDL_Texture* tex_tilebomb;
SDL_Texture* tex_flag;
SDL_Texture* tex_falseflag;

SDL_Texture* smileface;
SDL_Texture* winface;
SDL_Texture* lostface;

SDL_Texture* top_left;
SDL_Texture* top_right;
SDL_Texture* middle_left;
SDL_Texture* middle_right;
SDL_Texture* bottom_left;
SDL_Texture* bottom_right;
SDL_Texture* top_bottom;
SDL_Texture* middle;
SDL_Texture* left_right;

SDL_Texture* counter0;
SDL_Texture* counter1;
SDL_Texture* counter2;
SDL_Texture* counter3;
SDL_Texture* counter4;
SDL_Texture* counter5;
SDL_Texture* counter6;
SDL_Texture* counter7;
SDL_Texture* counter8;
SDL_Texture* counter9;
SDL_Texture* counter_minus;


bool init_graphics(tileset* logic){
    // Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "ERROR: Initializing SDL\n");
        return false;
    }

    window_height = logic->height * TILE_SIZE + BORDER_SIZE * 2 + HEADER_HEIGHT + BOARDER_MIDDLE;
    window_width = logic->width * TILE_SIZE + BORDER_SIZE * 2;

    // Create window
    window = SDL_CreateWindow(
        "Bombs", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        window_width,
        window_height,
        SDL_WINDOW_SHOWN
    );
    if(window == NULL){
        fprintf(stderr, "ERROR: Creating window\n");
        return false;
    }
    // this for some reason doesnt work
    SDL_SetWindowResizable(window, SDL_FALSE);

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(renderer == NULL){
        SDL_DestroyWindow(window);
        fprintf(stderr, "ERROR: Creating renderer\n");
        return false;
    }

    //load textures 
    tex_bomb        = loadTexture("img/tiles/bomb.svg", renderer);
    tex_tile0       = loadTexture("img/tiles/tile0.svg", renderer);
    tex_tile1       = loadTexture("img/tiles/tile1.svg", renderer);
    tex_tile2       = loadTexture("img/tiles/tile2.svg", renderer);
    tex_tile3       = loadTexture("img/tiles/tile3.svg", renderer);
    tex_tile4       = loadTexture("img/tiles/tile4.svg", renderer);
    tex_tile5       = loadTexture("img/tiles/tile5.svg", renderer);
    tex_tile6       = loadTexture("img/tiles/tile6.svg", renderer);
    tex_tile7       = loadTexture("img/tiles/tile7.svg", renderer);
    tex_tile8       = loadTexture("img/tiles/tile8.svg", renderer);
    tex_tilefull    = loadTexture("img/tiles/tilefull.svg", renderer);
    tex_tilebomb    = loadTexture("img/tiles/tilebomb.svg", renderer);
    tex_flag        = loadTexture("img/tiles/flag.svg", renderer);
    tex_falseflag   = loadTexture("img/tiles/falseflag.svg", renderer);

    smileface       = loadTexture("img/face/smileface.svg", renderer);  
    lostface        = loadTexture("img/face/lostface.svg", renderer);
    winface         = loadTexture("img/face/winface.svg", renderer);

    top_left        = loadTexture("img/border/topleft.svg", renderer);
    top_right       = loadTexture("img/border/topright.svg", renderer);
    middle_left     = loadTexture("img/border/middleleft.svg", renderer);
    middle_right    = loadTexture("img/border/middleright.svg", renderer);
    bottom_left     = loadTexture("img/border/bottomleft.svg", renderer);
    bottom_right    = loadTexture("img/border/bottomright.svg", renderer);
    top_bottom      = loadTexture("img/border/topbottom.svg", renderer);
    middle          = loadTexture("img/border/middle.svg", renderer);
    left_right      = loadTexture("img/border/leftright.svg", renderer);

    counter0        = loadTexture("img/counter/counter0.svg", renderer);
    counter1        = loadTexture("img/counter/counter1.svg", renderer);
    counter2        = loadTexture("img/counter/counter2.svg", renderer);
    counter3        = loadTexture("img/counter/counter3.svg", renderer);
    counter4        = loadTexture("img/counter/counter4.svg", renderer);
    counter5        = loadTexture("img/counter/counter5.svg", renderer);
    counter6        = loadTexture("img/counter/counter6.svg", renderer);
    counter7        = loadTexture("img/counter/counter7.svg", renderer);
    counter8        = loadTexture("img/counter/counter8.svg", renderer);
    counter9        = loadTexture("img/counter/counter9.svg", renderer);
    counter_minus   = loadTexture("img/counter/counter-.svg", renderer);

    // Set border rectangles
    rect_top.h = BORDER_SIZE;               rect_top.w = window_width;          rect_top.x = 0;                                     rect_top.y = 0;
    rect_middle.h = BOARDER_MIDDLE;         rect_middle.w = window_width;       rect_middle.x = 0;                                  rect_middle.y = BORDER_SIZE + HEADER_HEIGHT;
    rect_bottom.h = BORDER_SIZE;            rect_bottom.w = window_width;       rect_bottom.x = 0;                                  rect_bottom.y = window_height - BORDER_SIZE;
    rect_left.h = window_height;            rect_left.w = BORDER_SIZE;          rect_left.x = 0;                                    rect_left.y = 0;
    rect_right.h = window_height;           rect_right.w = BORDER_SIZE;         rect_right.x = window_width - BORDER_SIZE;          rect_right.y = 0;
    
    rect_top_left.h = BORDER_SIZE;          rect_top_left.w = BORDER_SIZE;      rect_top_left.x = 0;                                rect_top_left.y = 0;
    rect_top_right.h = BORDER_SIZE;         rect_top_right.w = BORDER_SIZE;     rect_top_right.x = window_width - BORDER_SIZE;      rect_top_right.y = 0;
    rect_middle_left.h = BOARDER_MIDDLE;    rect_middle_left.w = BORDER_SIZE;   rect_middle_left.x = 0;                             rect_middle_left.y = BORDER_SIZE + HEADER_HEIGHT;
    rect_middle_right.h = BOARDER_MIDDLE;   rect_middle_right.w = BORDER_SIZE;  rect_middle_right.x = window_width - BORDER_SIZE;   rect_middle_right.y = BORDER_SIZE + HEADER_HEIGHT;
    rect_bottom_left.h = BORDER_SIZE;       rect_bottom_left.w = BORDER_SIZE;   rect_bottom_left.x = 0;                             rect_bottom_left.y = window_height - BORDER_SIZE;
    rect_bottom_right.h = BORDER_SIZE;      rect_bottom_right.w = BORDER_SIZE;  rect_bottom_right.x = window_width - BORDER_SIZE;   rect_bottom_right.y = window_height - BORDER_SIZE;

    // render border rectangles
    SDL_RenderCopy(renderer, top_bottom, NULL, &rect_top);
    SDL_RenderCopy(renderer, middle, NULL, &rect_middle);
    SDL_RenderCopy(renderer, top_bottom, NULL, &rect_bottom);
    SDL_RenderCopy(renderer, left_right, NULL, &rect_left);
    SDL_RenderCopy(renderer, left_right, NULL, &rect_right);
    
    SDL_RenderCopy(renderer, top_left, NULL, &rect_top_left);
    SDL_RenderCopy(renderer, top_right, NULL, &rect_top_right);
    SDL_RenderCopy(renderer, middle_left, NULL, &rect_middle_left);
    SDL_RenderCopy(renderer, middle_right, NULL, &rect_middle_right);
    SDL_RenderCopy(renderer, bottom_left, NULL, &rect_bottom_left);
    SDL_RenderCopy(renderer, bottom_right, NULL, &rect_bottom_right);

    // Set rectangle for header and render them
    rect_header.h = HEADER_HEIGHT;
    rect_header.w = TILE_SIZE * logic->width;
    rect_header.x = BORDER_SIZE;
    rect_header.y = BORDER_SIZE;
    SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
    SDL_RenderFillRect(renderer, &rect_header);

    // Set rectangles for counters
    rect_bombs_1.h = DIGIT_HEIGHT; rect_bombs_1.w = DIGIT_WIDTH;        rect_bombs_1.x = BORDER_SIZE + (HEADER_HEIGHT-FACE_SIZE)/2 + DIGIT_WIDTH * 2;   rect_bombs_1.y = BORDER_SIZE + (HEADER_HEIGHT-FACE_SIZE)/2;
    rect_bombs_10.h = DIGIT_HEIGHT; rect_bombs_10.w = DIGIT_WIDTH;      rect_bombs_10.x = BORDER_SIZE + (HEADER_HEIGHT-FACE_SIZE)/2 + DIGIT_WIDTH;      rect_bombs_10.y = BORDER_SIZE + (HEADER_HEIGHT-FACE_SIZE)/2;
    rect_bombs_100.h = DIGIT_HEIGHT; rect_bombs_100.w = DIGIT_WIDTH;    rect_bombs_100.x = BORDER_SIZE + (HEADER_HEIGHT-FACE_SIZE)/2;                   rect_bombs_100.y = BORDER_SIZE + (HEADER_HEIGHT-FACE_SIZE)/2;

    rect_time_1.h = DIGIT_HEIGHT; rect_time_1.w = DIGIT_WIDTH;          rect_time_1.x = window_width - BORDER_SIZE - (HEADER_HEIGHT-FACE_SIZE)/2 - DIGIT_WIDTH;         rect_time_1.y = BORDER_SIZE + (HEADER_HEIGHT-FACE_SIZE)/2;
    rect_time_10.h = DIGIT_HEIGHT; rect_time_10.w = DIGIT_WIDTH;        rect_time_10.x = window_width - BORDER_SIZE - (HEADER_HEIGHT-FACE_SIZE)/2 - DIGIT_WIDTH * 2;    rect_time_10.y = BORDER_SIZE + (HEADER_HEIGHT-FACE_SIZE)/2;
    rect_time_100.h = DIGIT_HEIGHT; rect_time_100.w = DIGIT_WIDTH;      rect_time_100.x = window_width - BORDER_SIZE - (HEADER_HEIGHT-FACE_SIZE)/2 - DIGIT_WIDTH * 3;   rect_time_100.y = BORDER_SIZE + (HEADER_HEIGHT-FACE_SIZE)/2;



    // Set rectangle for face
    rect_face.h = FACE_SIZE;
    rect_face.w = FACE_SIZE;
    rect_face.x = window_width/2 - FACE_SIZE/2;
    rect_face.y = BORDER_SIZE + HEADER_HEIGHT/2 - FACE_SIZE/2;

    // Define tile rectangles
    rect_tiles = malloc(sizeof(SDL_Rect*) * logic->height);
    if(rect_tiles == NULL){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        fprintf(stderr, "ERROR: Allocating structures for rectangles 1\n");
        return false;
    }
    for(int i = 0; i < logic->height; i++){
        rect_tiles[i] = malloc(sizeof(SDL_Rect) * logic->width);
        if(rect_tiles[i] == NULL){
            for(int k = i - 1; k >= 0; k--){
                free(rect_tiles[k]);
            }
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            fprintf(stderr, "ERROR: Allocating structures for rectangles 2\n");
            return false;
        }
    }
    // Set rectangles position and size
    for(int i = 0; i < logic->height; i++){
        for(int j = 0; j < logic->width; j++){
            rect_tiles[i][j].h = TILE_SIZE;
            rect_tiles[i][j].w = TILE_SIZE;
            rect_tiles[i][j].x = BORDER_SIZE + j * TILE_SIZE;
            rect_tiles[i][j].y = BORDER_SIZE + HEADER_HEIGHT + BOARDER_MIDDLE + i * TILE_SIZE;
        }
    }


    update_graphics(logic);

    return true;
}

// loads a image from a path to the texture
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer){
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if(texture == NULL){
        fprintf(stderr, "ERROR: Unable to create texture from %s . IMG error: %s\n", path, IMG_GetError());
        return NULL;
    }
    return texture;

}

// returns the correct texture for given number
SDL_Texture* numbertoTexture(int number){
    switch(number){
        case 0:
            return counter0;
        case 1:
            return counter1;
        case 2:
            return counter2;
        case 3:
            return counter3;
        case 4:
            return counter4;
        case 5:
            return counter5;
        case 6:
            return counter6;
        case 7:
            return counter7;
        case 8:
            return counter8;
        case 9:
            return counter9;
        default:
            return NULL;
    }
}

// updates all dynamic segments of the graphics
void update_graphics(tileset* logic){
    // update face
    if(logic->state == LOGIC_LOSS){
        SDL_RenderCopy(renderer, lostface, NULL, &rect_face);
    }else if(logic->state == LOGIC_WIN){
        SDL_RenderCopy(renderer, winface, NULL, &rect_face);
    }else{
        SDL_RenderCopy(renderer, smileface, NULL, &rect_face);
    }

    // update bomb counter
    SDL_RenderCopy(renderer, numbertoTexture(abs(logic->bombs_remaining) % 1000 / 100), NULL, &rect_bombs_100);
    SDL_RenderCopy(renderer, numbertoTexture(abs(logic->bombs_remaining) % 100 / 10), NULL, &rect_bombs_10);
    SDL_RenderCopy(renderer, numbertoTexture(abs(logic->bombs_remaining) % 10), NULL, &rect_bombs_1);
    if(logic->bombs_remaining < 0 && logic->bombs_remaining > -10){
        SDL_RenderCopy(renderer, counter_minus, NULL, &rect_bombs_10);
    }else if(logic->bombs_remaining < 0 && logic->bombs_remaining > -100){
        SDL_RenderCopy(renderer, counter_minus, NULL, &rect_bombs_100);
    }

    // update time counter
    SDL_RenderCopy(renderer, numbertoTexture(logic->time % 1000 / 100), NULL, &rect_time_100);
    SDL_RenderCopy(renderer, numbertoTexture(logic->time % 100 / 10), NULL, &rect_time_10);
    SDL_RenderCopy(renderer, numbertoTexture(logic->time % 10), NULL, &rect_time_1);

    // update tiles
    for(int i = 0; i < logic->height; i++){
        for(int j = 0; j < logic->width; j++){
            if(logic->field[i][j].mask == HIDDEN){
                SDL_RenderCopy(renderer, tex_tilefull, NULL, &rect_tiles[i][j]);
            }else if(logic->field[i][j].mask == FLAG){
                if(logic->state == LOGIC_LOSS && logic->field[i][j].value != BOMB){
                    SDL_RenderCopy(renderer, tex_falseflag, NULL, &rect_tiles[i][j]);
                }else{
                    SDL_RenderCopy(renderer, tex_flag, NULL, &rect_tiles[i][j]);
                }
            }else{
                switch(logic->field[i][j].value){
                    case BLANK:
                        SDL_RenderCopy(renderer, tex_tile0, NULL, &rect_tiles[i][j]);
                        break;
                    case 1:
                        SDL_RenderCopy(renderer, tex_tile1, NULL, &rect_tiles[i][j]);
                        break;
                    case 2:
                        SDL_RenderCopy(renderer, tex_tile2, NULL, &rect_tiles[i][j]);
                        break;
                    case 3:
                        SDL_RenderCopy(renderer, tex_tile3, NULL, &rect_tiles[i][j]);
                        break;
                    case 4:
                        SDL_RenderCopy(renderer, tex_tile4, NULL, &rect_tiles[i][j]);
                        break;
                    case 5:
                        SDL_RenderCopy(renderer, tex_tile5, NULL, &rect_tiles[i][j]);
                        break;
                    case 6:
                        SDL_RenderCopy(renderer, tex_tile6, NULL, &rect_tiles[i][j]);
                        break;
                    case 7:
                        SDL_RenderCopy(renderer, tex_tile7, NULL, &rect_tiles[i][j]);
                        break;
                    case 8:
                        SDL_RenderCopy(renderer, tex_tile8, NULL, &rect_tiles[i][j]);
                        break;
                    case BOMB:
                        SDL_RenderCopy(renderer, tex_tilebomb, NULL, &rect_tiles[i][j]);
                        break;
                    case TRIGGERED_BOMB:
                        SDL_RenderCopy(renderer, tex_bomb, NULL, &rect_tiles[i][j]);
                        break;
                    default:

                }
            }
        }
    }
    // render the result
    SDL_RenderPresent(renderer);
}

// function to process keyboard/mouse input and countdown. Return true, if the graphics of the game needs to be updated
bool process_input(tileset* logic, int *row, int *col, int *key){
    // first check if there is any event to process
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type){

        // game was terminated with command
        case SDL_QUIT:
            game_quit = false;
            break;

        // ESC key was pressed
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE){
                game_quit = false;
            }
            break;

        // mouse input
        case SDL_MOUSEBUTTONDOWN:
            int x, y;
            SDL_GetMouseState(&x, &y);
            // if face icon was pressed, restart the game
            if(x > rect_face.x && x < rect_face.x + FACE_SIZE && y > rect_face.y && y < rect_face.y + FACE_SIZE){
                logic->bombs_remaining = logic->bombs;
                logic->time = 0;
                logic->time_is_running = false;
                logic->state = LOGIC_START;
                hide_tiles(logic);
                return true;
            }
            // else, check if any of the tiles was pressed
            for(int i = 0; i < logic->height; i++){
                for(int j = 0; j < logic->width; j++){
                    if(x > rect_tiles[i][j].x && x < rect_tiles[i][j].x + TILE_SIZE &&
                        y > rect_tiles[i][j].y && y < rect_tiles[i][j].y + TILE_SIZE){
                        // if yes, return the row, column and key values
                        *row = i;
                        *col = j;
                        if(event.button.button == SDL_BUTTON_LEFT){
                            *key = LEFT_KEY;
                            return true;
                        }else if(event.button.button == SDL_BUTTON_RIGHT){
                            *key = RIGHT_KEY;
                            return true;
                        }
                        return false;
                    }
                }
            }
            break;

        default:
            break;
    }

    // when the keyboard/mouse input is processed, try updating the countdown.
    // Update the time, if time is running, and one second passed since the last update
    if(logic->time_is_running){
        int current_second = (int)((SDL_GetTicks() - logic->start_time) / 1000);
        if(current_second > logic->time){
            logic->time = current_second;
            return true;
        }
    }
    return false;
}

void destroy_graphics(void){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
