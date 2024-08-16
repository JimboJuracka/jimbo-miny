#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "game_graphics.h"
#include "game_logic.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Rect** rect_tiles;
SDL_Rect rect_face;
SDL_Rect rect_header;

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


bool init_graphics(tileset* logic){
    // Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "ERROR: Initializing SDL\n");
        return false;
    }

    // Create window
    window = SDL_CreateWindow(
        "Bombs", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        logic->width * TILE_SIZE + BORDER_AROUND_BOARD * 2,
        logic->height * TILE_SIZE + BORDER_AROUND_BOARD * 2 + BOARD_HEADER_SIZE + BOARDER_MIDDLE_SIZE,
        0
    );
    if(window == NULL){
        fprintf(stderr, "ERROR: Creating window\n");
        return false;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(renderer == NULL){
        SDL_DestroyWindow(window);
        fprintf(stderr, "ERROR: Creating renderer\n");
        return false;
    }

    //load textures 
    tex_bomb        = loadTexture("img/bomb.svg", renderer);
    tex_tile0       = loadTexture("img/tile0.svg", renderer);
    tex_tile1       = loadTexture("img/tile1.svg", renderer);
    tex_tile2       = loadTexture("img/tile2.svg", renderer);
    tex_tile3       = loadTexture("img/tile3.svg", renderer);
    tex_tile4       = loadTexture("img/tile4.svg", renderer);
    tex_tile5       = loadTexture("img/tile5.svg", renderer);
    tex_tile6       = loadTexture("img/tile6.svg", renderer);
    tex_tile7       = loadTexture("img/tile7.svg", renderer);
    tex_tile8       = loadTexture("img/tile8.svg", renderer);
    tex_tilefull    = loadTexture("img/tilefull.svg", renderer);
    tex_tilebomb    = loadTexture("img/tilebomb.svg", renderer);
    tex_flag        = loadTexture("img/flag.svg", renderer);
    tex_falseflag   = loadTexture("img/falseflag.svg", renderer);
    smileface       = loadTexture("img/smileface.svg", renderer);  
    lostface        = loadTexture("img/lostface.svg", renderer);
    winface         = loadTexture("img/winface.svg", renderer);


    // Define rectangles
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
            rect_tiles[i][j].x = BORDER_AROUND_BOARD + j * TILE_SIZE;
            rect_tiles[i][j].y = BORDER_AROUND_BOARD + BOARD_HEADER_SIZE + BOARDER_MIDDLE_SIZE + i * TILE_SIZE;
        }
    }

    // Set background
    SDL_SetRenderDrawColor(renderer, 0, 200, 200, 255);
    SDL_RenderClear(renderer);

    // Set rectangle for header
    rect_header.h = BOARD_HEADER_SIZE;
    rect_header.w = TILE_SIZE * logic->width;
    rect_header.x = BORDER_AROUND_BOARD;
    rect_header.y = BORDER_AROUND_BOARD;
    SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
    SDL_RenderFillRect(renderer, &rect_header);

    // Set rectangle for face
    rect_face.h = FACE_SIZE;
    rect_face.w = FACE_SIZE;
    rect_face.x = (logic->width * TILE_SIZE + BORDER_AROUND_BOARD * 2)/2 - FACE_SIZE/2;
    rect_face.y = BORDER_AROUND_BOARD + BOARD_HEADER_SIZE/2 - FACE_SIZE/2;
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect_face);

    update_graphics(logic);

    return true;
}

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer){
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if(texture == NULL){
        fprintf(stderr, "ERROR: Unable to create texture from %s . IMG error: %s\n", path, IMG_GetError());
        return NULL;
    }
    return texture;

}

void update_graphics(tileset* logic){
    // update face
    if(game_state == LOGIC_IDLE){
        SDL_RenderCopy(renderer, smileface, NULL, &rect_face);
    }else if(game_state == LOGIC_LOSS){
        SDL_RenderCopy(renderer, lostface, NULL, &rect_face);
    }else if(game_state == LOGIC_WIN){
        SDL_RenderCopy(renderer, winface, NULL, &rect_face);
    }
    
    // update tiles
    for(int i = 0; i < logic->height; i++){
        for(int j = 0; j < logic->width; j++){
            if(logic->field[i][j].mask == HIDDEN){
                SDL_RenderCopy(renderer, tex_tilefull, NULL, &rect_tiles[i][j]);
            }else if(logic->field[i][j].mask == FLAG){
                if(game_state == LOGIC_LOSS && logic->field[i][j].value != BOMB){
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
    SDL_RenderPresent(renderer);
}

//returns true if one of the tiles was clicked on, with left or right mouse button
bool process_input(tileset* logic, int *row, int *col, int *key){
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type){
        case SDL_QUIT:
            game_quit = false;
            break;

        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE){
                game_quit = false;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            int x, y;
            SDL_GetMouseState(&x, &y);
            if(x > rect_face.x && x < rect_face.x + FACE_SIZE && y > rect_face.y && y < rect_face.y + FACE_SIZE){
                setup_game(logic);
                game_state = LOGIC_IDLE;
                // TODO: reset timer
                return false;
            }
            for(int i = 0; i < logic->height; i++){
                for(int j = 0; j < logic->width; j++){
                    if(x > rect_tiles[i][j].x && x < rect_tiles[i][j].x + TILE_SIZE &&
                        y > rect_tiles[i][j].y && y < rect_tiles[i][j].y + TILE_SIZE){
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
    return false;
}

void destroy_graphics(void){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
