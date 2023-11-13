#include "extension.h"

int main(int argc, char* argv[]) 
{
    test();
    validate_arg_count(argc);
    bool is_verbose = false;
    int size;
    parse_args(argc, argv, &size, &is_verbose);
    Board solutions[SMALL_LIST];
    int solution_count = get_solutions(size, solutions);
    if(is_verbose){
        draw_boards(solutions, solution_count);
    }
    printf("%i solutions \n", solution_count);
    return 0;
}

void draw_boards(Board solutions[SMALL_LIST], int num_solutions)
{
    SDL_Simplewin sw;        
    Neill_SDL_Init(&sw);
    int i = 0;
    do {
        sdl_draw_board(solutions[i], &sw);
        SDL_Delay(MILLISECONDDELAY);
        i++;
    } while (!sw.finished && i<num_solutions);
    SDL_Quit();
    atexit(SDL_Quit);
}

void sdl_draw_board(Board b, SDL_Simplewin * sw)
{
    SDL_Rect rectangle;
    rectangle.w = RECTSIZE;
    rectangle.h = RECTSIZE;
    draw_chess_board(b.size, rectangle, sw);
    for (int r = 0; r < b.size; r++) {
        for (int c = 0; c < b.size; c++) {
            if (b.grid[r][c] == 'Q') {
                draw_queen(sw, r, c);
            }
        }
    }
    Neill_SDL_UpdateScreen(sw);
    Neill_SDL_Events(sw);
}

void draw_queen(SDL_Simplewin * sw, int y, int x)
{
    // convert to pixels.
    x=x*QUEEN_SIZE;
    y=y*QUEEN_SIZE;
    // base
    draw_yellow_rectangle(QUEEN_BASE_W, QUEEN_BASE_H, 
                          QUEEN_BASE_X_OFFSET + x,
                          QUEEN_BASE_Y_OFFSET + y, sw);
    draw_yellow_rectangle(QUEEN_SPINE_W, QUEEN_SPINE_H,
                          QUEEN_SPINE_1_X_OFFSET + x,
                          QUEEN_SPINE_1_Y_OFFSET + y, sw);
    draw_yellow_rectangle(QUEEN_SPINE_2_W, QUEEN_SPINE_2_H,
                          QUEEN_SPINE_2_X_OFFSET + x,
                          QUEEN_SPINE_2_Y_OFFSET + y, sw);
    draw_yellow_rectangle(QUEEN_SPINE_W, QUEEN_SPINE_H,
                          QUEEN_SPINE_3_X_OFFSET + x,
                          QUEEN_SPINE_3_Y_OFFSET + y, sw);
}

void draw_yellow_rectangle(int w, int h, int x, int y, SDL_Simplewin * sw)
{
    Neill_SDL_SetDrawColour(sw, YELLOW);

    SDL_Rect rect;
    rect.w = w*QUEEN_SIZE;
    rect.h = h*QUEEN_SIZE;
    rect.x = x*QUEEN_SIZE; 
    rect.y = y*QUEEN_SIZE;
    SDL_RenderFillRect(sw->renderer, &rect);
}

void draw_chess_board(int size, SDL_Rect rectangle, SDL_Simplewin * sw) 
{
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            rectangle.x = x*RECTSIZE;
            rectangle.y = y*RECTSIZE;
            if((x+y)%2==0){
                Neill_SDL_SetDrawColour(sw, WHITE);
                SDL_RenderFillRect(sw->renderer, &rectangle);
            }
            else{
                Neill_SDL_SetDrawColour(sw, BLACK);
                SDL_RenderFillRect(sw->renderer, &rectangle);
            }
        }
    }
}