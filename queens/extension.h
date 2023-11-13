#include "8q.h"
#include "neillsdl2.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void draw_chess_board(int size, 
                    SDL_Rect rectangle, 
                    SDL_Simplewin * sw);
void draw_queen(SDL_Simplewin * sw, int x, int y);
void draw_yellow_rectangle( int w, int h, 
                            int x, int y, 
                            SDL_Simplewin * sw);
void sdl_draw_board(Board b, SDL_Simplewin * sw);
void draw_boards(Board solutions[MEDIUM_LIST], 
                int num_solutions);
void parse_ext_args(int argc, char* argv[], int* size, 
                        bool * is_visualize);
void ext_solve(int row, Board *current_board, Board solutions[MEDIUM_LIST], int *num_solutions);
#define RECTSIZE 100
#define MILLISECONDDELAY 2000
#define WHITE 255, 255, 255
#define YELLOW 255, 255, 0
#define BLACK 0, 0, 0
#define QUEEN_SIZE 10
#define QUEEN_BASE_W 6
#define QUEEN_BASE_H 3 
#define QUEEN_BASE_X_OFFSET 2
#define QUEEN_BASE_Y_OFFSET 5

#define QUEEN_SPINE_W 1
#define QUEEN_SPINE_H 3
#define QUEEN_SPINE_1_X_OFFSET 2
#define QUEEN_SPINE_1_Y_OFFSET 2

#define QUEEN_SPINE_2_W 2
#define QUEEN_SPINE_2_H 3
#define QUEEN_SPINE_2_X_OFFSET 4
#define QUEEN_SPINE_2_Y_OFFSET 2

#define QUEEN_SPINE_3_X_OFFSET 7
#define QUEEN_SPINE_3_Y_OFFSET 2

#define CORRECT_EXT_USAGE "Usage:\n ./8q 6 \n ./8q -visualize 3 \n"