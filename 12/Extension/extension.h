#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include "../General/general.h"
#include "nsdl/neillsdl2.h"

#define WHITE 255, 255, 255
#define YELLOW 255, 255, 0
#define BLACK 0, 0, 0
#define GREEN 0, 255, 0
#define RED 255, 0, 0
#define BLUE 0, 0, 255
#define CYAN 0, 255, 255
#define MAGENTA 255, 0, 255
#define DARK_GREEN 0, 100, 0

#define DEGREES_IN_CIRC 360

#define STEP_SIZE 5
#define START_X (WWIDTH - RECTSIZE) / 2
#define START_Y (WHEIGHT - RECTSIZE) / 2


void do_input(SDL_Simplewin *sw);
void do_key_down(SDL_KeyboardEvent *event, SDL_Simplewin *sw);
void do_key_up(SDL_KeyboardEvent *event, SDL_Simplewin *sw);
void draw(SDL_Simplewin *sw);
void set_draw_colour(SDL_Simplewin *sw, neillcol colour);
void move_ttl(SDL_Simplewin *sw);
void init_ttl_vars(SDL_Simplewin *sw);
void draw_trail(SDL_Simplewin *sw, int orig_x, int orig_y);
