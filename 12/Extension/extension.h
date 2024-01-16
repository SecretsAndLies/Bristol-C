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
#define GREEN 0, 100, 0

void do_input(SDL_Simplewin *sw);
void do_key_down(SDL_KeyboardEvent *event, SDL_Simplewin *sw);
void do_key_up(SDL_KeyboardEvent *event, SDL_Simplewin *sw);
void set_draw_colour(SDL_Simplewin *sw, int red, int green, int blue);

