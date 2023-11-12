#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "neillsdl2.h"

#define RECTSIZE 100
#define MILLISECONDDELAY 20
#define WHITE 255, 255, 255
#define YELLOW 255, 255, 0
#define QUEEN_SIZE 10


int main(void) 
{
    SDL_Simplewin sw;
    SDL_Rect rectangle;
    rectangle.w = RECTSIZE;
    rectangle.h = RECTSIZE;

    Neill_SDL_Init(&sw);
    do {
        SDL_Delay(MILLISECONDDELAY);
        // TODO MAKE 4 a number passed in.
        draw_chess_board(4, rectangle, sw);
        // TODO loop through board adding queens.
        draw_queen(sw, 1, 1);
        Neill_SDL_UpdateScreen(&sw);
        Neill_SDL_Events(&sw);
    } while (!sw.finished);

    /* Clear up graphics subsystems */
    SDL_Quit();
    atexit(SDL_Quit);

    return 0;
}

void draw_queen(SDL_Simplewin sw, int y, int x)
{
    // convert to pixels.
    x=x*QUEEN_SIZE;
    y=y*QUEEN_SIZE;
    // base
    // TODO ask about magic.
    draw_yellow_rectangle(6, 3, 2+x, 5+y, sw);
    // spine 1
    draw_yellow_rectangle(1, 3, 2+x, 2+y, sw);
    // // spine middle
    draw_yellow_rectangle(2, 3, 4+x, 2+y, sw);
    // // spine 2
    draw_yellow_rectangle(1, 3, 7+x, 2+y, sw);
}

void draw_yellow_rectangle(int w, int h, int x, int y, SDL_Simplewin sw){
    Neill_SDL_SetDrawColour(&sw, YELLOW);

    SDL_Rect rect;
    rect.w = w*QUEEN_SIZE;
    rect.h = h*QUEEN_SIZE;
    rect.x = x*QUEEN_SIZE; 
    rect.y = y*QUEEN_SIZE;
    SDL_RenderFillRect(sw.renderer, &rect);
}

void draw_chess_board(int size, SDL_Rect rectangle, SDL_Simplewin sw) 
{
    Neill_SDL_SetDrawColour(&sw, WHITE);
    bool draw_white = false;
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            rectangle.x = x*RECTSIZE;
            rectangle.y = y*RECTSIZE;
            if(draw_white){
                SDL_RenderFillRect(sw.renderer, &rectangle);
                draw_white=false;
            }
            else{
                SDL_RenderDrawRect(sw.renderer, &rectangle);
                draw_white=true;
            }
            // special case, if you're at the end of the col
            // the next one you draw needs to be the same color as this.
            if(y==size-1){
                draw_white=!draw_white;
            }
        }
    }
}