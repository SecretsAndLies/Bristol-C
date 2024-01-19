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
#define CIRCLE_RAD 10

typedef struct {
   int up;
   int right;
} Controls;

enum neillcol {black=30, red, green, yellow, blue, magenta, cyan, white, dark_green};
typedef enum neillcol neillcol;

typedef struct {
   int x;
   int y;
   int angle;
   neillcol colour;
   char instruction[200];
   int ins_amount;
   Controls ctl;
   bool finished;
} Turtle;

void do_input(Turtle * ttl);
void do_key_down(SDL_KeyboardEvent *event, Turtle * ttl);
void do_key_up(SDL_KeyboardEvent *event, Turtle * ttl);
void draw(SDL_Simplewin *sw, Turtle * ttl);
void set_draw_colour(SDL_Simplewin *sw, neillcol colour);
void move_ttl(Turtle * ttl);
Turtle * init_ttl_vars(void);
void draw_trail(SDL_Simplewin *sw, int orig_x, int orig_y, Turtle * ttl);
void save_inst(SDL_Simplewin *sw, char * inst, int amount, char * col);
void print_inst_if_exists(Turtle * ttl);
void draw_ttl(SDL_Simplewin *sw, Turtle * ttl);
void test(void);
void go_rgt(Turtle * ttl);
void go_fwd(Turtle * ttl, double radians);
void* acalloc(int n, size_t size);